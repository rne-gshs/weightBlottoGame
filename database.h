//
// Created by cygnus330 on 2023-04-05.
//

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <iostream>
#include <cstdio>
#include <utility>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cstdint>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <random>

const double E = (double)2.7182818284590452354;

namespace db
{
	//결과저장
	class chainDataBase
	{
	 protected:
		struct wdl
		{
			uint32_t win;
			uint32_t draw;
			uint32_t lose;

			wdl()
			{
				win = 0;
				draw = 0;
				lose = 0;
			}
			wdl(uint32_t w, uint32_t d, uint32_t l) : win(w), draw(d), lose(l) {};

			uint32_t sum() { return (win + draw + lose); }

			bool operator < (const wdl &a) const
			{
				if(this->win == a.win)
				{
					if(this->draw == a.draw)
						return (this->lose < a.lose);

					return (this->draw < a.draw);
				}

				return (this->win < a.win);
			}

			bool operator == (const wdl &a) const
			{
				return (this->win == a.win && this->draw == a.draw && this->lose == a.lose);
			}
		};

		double lambda;
		double WRsum = 0;
		uint32_t size = 0;
		std::vector<double> vs;
		std::vector<std::vector<std::uint32_t>> vc;

		std::map<std::vector<uint32_t>, wdl> DB_vecToWdl;
		bool isChanged;

	 public:
		chainDataBase()
		{
			this->isChanged = true;
			this->DB_vecToWdl.clear();
			lambda = 1;
		}
		chainDataBase(const double l)
		{
			this->isChanged = true;
			this->DB_vecToWdl.clear();
			lambda = l;
		}
		chainDataBase(const chainDataBase& ori)
		{
			this->WRsum = ori.WRsum;
			this->size = ori.size;
			this->vs = ori.vs;
			this->vc = ori.vc;

			this->isChanged = ori.isChanged;
			this->DB_vecToWdl = ori.DB_vecToWdl;
			lambda = 1;
		}
		chainDataBase(const chainDataBase& ori, const double l)
		{
			this->WRsum = ori.WRsum;
			this->size = ori.size;
			this->vs = ori.vs;
			this->vc = ori.vc;

			this->isChanged = ori.isChanged;
			this->DB_vecToWdl = ori.DB_vecToWdl;
			lambda = l;
		}

		void push(const uint32_t w, const uint32_t d, const uint32_t l, const std::vector<uint32_t>& vpush)
		{
			isChanged = true;

			wdl T = wdl(w, d, l);

			auto vtw = std::make_pair(vpush, T);
			DB_vecToWdl.insert(vtw);
		}
		void mod(const std::vector<uint32_t>& vmod, const uint32_t n, std::string type)
		{
			isChanged = true;

			wdl* pm = &DB_vecToWdl[vmod];
			if(type == "win")
				pm->win += n;
			else if(type == "draw")
				pm->draw += n;
			else if(type == "lose")
				pm->lose += n;
			else
			{
				std::cerr << "Wrong Query " << type << std::endl;
				exit(10011);
			}

		}
		std::vector<uint32_t> getTarget()
		{
			if(isChanged)
			{
				isChanged = false;
				vc.resize(0);
				vs.assign(1, 0.0);

				WRsum = 0;
				size = 0;

				for(auto& i:DB_vecToWdl)
				{
					uint32_t winCnt = i.second.win;
					uint32_t gameCnt = winCnt + i.second.lose + i.second.draw;
					double WR = (double)winCnt / (double)gameCnt;
					WR = std::pow(E, lambda * WR);

					size++;
					WRsum += WR;

					vs.push_back(WR + vs.back());
					vc.emplace_back(i.first);
				}
			}

			std::random_device _rd;
			std::uniform_real_distribution<double> _uniform(0, WRsum);
			std::mt19937 _engine(_rd());
			double x = _uniform(_engine);
			unsigned int x_ptr = std::lower_bound(vs.begin(), vs.end(), x) - vs.begin();

			return vc[x_ptr - 1];
		}

		std::vector<std::tuple<std::vector<std::uint32_t>, std::array<std::uint32_t, 3>, double>> write()
		{
			struct vectorContainer
			{
				std::vector<uint32_t> vec;
				wdl w;
				double P;

				vectorContainer(std::vector<uint32_t> a, wdl b, double c) : vec(std::move(a)), w(b), P(c) {}
			};

			double psum = 0;
			std::vector<vectorContainer> originalBuffer;
			for(auto& i:DB_vecToWdl)
			{
				double s = i.second.sum();
				double p = pow(E, lambda * (i.second.win / s));
				psum += p;
				originalBuffer.emplace_back(i.first, i.second, p);
			}

			auto comp = [](auto &a, auto &b)->bool{return !(a.w < b.w);};
			std::sort(originalBuffer.begin(), originalBuffer.end(), comp);

			std::vector<std::tuple<std::vector<std::uint32_t>, std::array<std::uint32_t, 3>, double>> buffer;

			for(auto &i:originalBuffer)
				buffer.emplace_back(i.vec, std::array<std::uint32_t, 3>{i.w.win, i.w.draw, i.w.lose}, i.P / psum);

			return buffer;

			/*std::vector<std::pair<std::vector<std::uint32_t>, double>> buffer;
			for(auto &i:originalBuffer)
			{
				buffer.emplace_back(i.vec, i.P / psum);
				buffer.back().first.emplace_back(i.w.win);
				buffer.back().first.emplace_back(i.w.draw);
				buffer.back().first.emplace_back(i.w.lose);
			}

			return buffer;*/
		}

		void setLambda(const double l) { lambda = l; }
	};
}

#endif //_DATABASE_H_
