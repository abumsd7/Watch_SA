#include "plugin.h"
#include "CFileLoader.h"
#include "CSprite2d.h"
#include <iomanip>
#include <iostream>
#include "common.h"
#include "CTimer.h"
#include "CAnimManager.h"
#include "eAnimations.h"
#include "CStreaming.h"
#include "CTaskSimpleRunNamedAnim.h"
using namespace plugin;

class Watch_SA {
public:
    Watch_SA() {
		static RwTexDictionary *watch = nullptr;
		static CSprite2d  ht, mt, bg, days;
		Events::initRwEvent += [&] {
			//config_file conf(PLUGIN_PATH("recthud.ini"));
			watch = CFileLoader::LoadTexDictionary(GAME_PATH("models\\watch_sa.txd"));
			ht.m_pTexture = RwTexDictionaryFindNamedTexture(watch, "hr");
			mt.m_pTexture = RwTexDictionaryFindNamedTexture(watch, "min");
			bg.m_pTexture = RwTexDictionaryFindNamedTexture(watch, "watch");

			Events::drawingEvent += [&] {
				static char hr, min, da;
				hr = patch::GetUChar(11993427, 1);
				min = patch::GetUChar(11993426, 1);
				da = patch::GetUChar(11993422,1);
				int day = int(da);
				char week[7][4] = { "sun", "mon" ,"tue","wed","thu","fri","sat" };
				days.m_pTexture = RwTexDictionaryFindNamedTexture(watch, week[day-1]);
				double h, hrr, mi, minn, p1, p2, p3, p4, p5, p6, p7, p8, size1, size2;
				double q1, q2, q3, q4, q5, q6, q7, q8;
				h = (double)(hr % 12);
				hrr = (h * 30 * 3.14) / 180.0;
				minn = (double)(min * 6);
				mi = (minn * 3.14) / 180.0;
				size1 = 75.0;
				size2 = 175.0;
				//hr needle
				p1 = cos(hrr + (mi / 12.0))*size1 - sin(hrr + (mi / 12.0)) * (size1)+size2;
				p2 = sin(hrr + (mi / 12.0))*size1 + cos(hrr + (mi / 12.0))* (size1)+size2;
				p3 = cos(hrr + (mi / 12.0))*size1 - sin(hrr + (mi / 12.0)) * (-size1) + size2;
				p4 = sin(hrr + (mi / 12.0))*size1 + cos(hrr + (mi / 12.0))* (-size1) + size2;
				p5 = cos(hrr + (mi / 12.0))*(-size1) - sin(hrr + (mi / 12.0)) * size1 + size2;
				p6 = sin(hrr + (mi / 12.0))*(-size1) + cos(hrr + (mi / 12.0))* size1 + size2;
				p7 = cos(hrr + (mi / 12.0))*(-size1) - sin(hrr + (mi / 12.0)) * (-size1) + size2;
				p8 = sin(hrr + (mi / 12.0))*(-size1) + cos(hrr + (mi / 12.0))* (-size1) + size2;
				//min needle
				q1 = cos(mi)*size1 - sin(mi) * (size1)+size2;
				q2 = sin(mi)*size1 + cos(mi)* (size1)+size2;
				q3 = cos(mi)*size1 - sin(mi) * (-size1) + size2;
				q4 = sin(mi)*size1 + cos(mi)* (-size1) + size2;
				q5 = cos(mi)*(-size1) - sin(mi) * size1 + size2;
				q6 = sin(mi)*(-size1) + cos(mi)* size1 + size2;
				q7 = cos(mi)*(-size1) - sin(mi) * (-size1) + size2;
				q8 = sin(mi)*(-size1) + cos(mi)* (-size1) + size2;
				//drawing part
				config_file conf(PLUGIN_PATH("watch.ini"));
				double wl, wr, wt, wb, dl, dr, dt, db;
				int	hl, hb, ml, mb;
				wl = conf["WatchScaleLeft"].asFloat(304.0f);
				wr = conf["WatchScaleRight"].asFloat(86.0f);
				wt = conf["WatchScaleTop"].asFloat(430.0f);
				wb = conf["WatchScaleBottom"].asFloat(20.0f);
				dl = conf["DayScaleLeft"].asFloat(180.0f);
				dr = conf["DayScaleRight"].asFloat(145.0f);
				dt = conf["DayScaleTop"].asFloat(236.0f);
				db = conf["DayScaleBottom"].asFloat(213.0f);
				hl = conf["HrNeedleUpDown"].asInt(54);
				hb = conf["HrNeedleLeftRight"].asInt(27);
				ml = conf["MinNeedleUpDown"].asInt(54);
				mb = conf["MinNeedleLeftRight"].asInt(27);
				if (conf["AlwaysON"].asInt() == 1)
				{
					//bg.Draw(SCREEN_RECT_BOTTOM_RIGHT(300.0, 430.0, 90.0, 20.0), CRGBA(255, 255, 255, 192));
					bg.Draw(SCREEN_RECT_BOTTOM_RIGHT(wl, wt, wr, wb), CRGBA(255, 255, 255, 255));
					days.Draw(SCREEN_RECT_BOTTOM_RIGHT(dl, dt, dr, db), CRGBA(255, 255, 255, 255));
					ht.Draw(SCREEN_COORD_RIGHT(p1 + hb), SCREEN_COORD_BOTTOM(p2 + hl), SCREEN_COORD_RIGHT(p3 + hb),
						SCREEN_COORD_BOTTOM(p4 + hl), SCREEN_COORD_RIGHT(p5 + hb), SCREEN_COORD_BOTTOM(p6 + hl),
						SCREEN_COORD_RIGHT(p7 + hb), SCREEN_COORD_BOTTOM(p8 + hl), CRGBA(255, 255, 255, 255));
					mt.Draw(SCREEN_COORD_RIGHT(q1 + mb), SCREEN_COORD_BOTTOM(q2 + ml), SCREEN_COORD_RIGHT(q3 + mb),
						SCREEN_COORD_BOTTOM(q4 + ml), SCREEN_COORD_RIGHT(q5 + mb), SCREEN_COORD_BOTTOM(q6 + ml),
						SCREEN_COORD_RIGHT(q7 + mb), SCREEN_COORD_BOTTOM(q8 + ml), CRGBA(255, 255, 255, 255));
				}
				else if (conf["AlwaysON"].asInt() == 2)
				{

					static int delayTime;
					if (KeyPressed('Q')) {
						delayTime = CTimer::m_snTimeInMilliseconds + 3000;
					}
					if ((CTimer::m_snTimeInMilliseconds < delayTime))
					{
						bg.Draw(SCREEN_RECT_BOTTOM_RIGHT(wl, wt, wr, wb), CRGBA(255, 255, 255, 255));
						days.Draw(SCREEN_RECT_BOTTOM_RIGHT(dl, dt, dr, db), CRGBA(255, 255, 255, 255));
						ht.Draw(SCREEN_COORD_RIGHT(p1 + hb), SCREEN_COORD_BOTTOM(p2 + hl), SCREEN_COORD_RIGHT(p3 + hb),
							SCREEN_COORD_BOTTOM(p4 + hl), SCREEN_COORD_RIGHT(p5 + hb), SCREEN_COORD_BOTTOM(p6 + hl),
							SCREEN_COORD_RIGHT(p7 + hb), SCREEN_COORD_BOTTOM(p8 + hl), CRGBA(255, 255, 255, 255));
						mt.Draw(SCREEN_COORD_RIGHT(q1 + mb), SCREEN_COORD_BOTTOM(q2 + ml), SCREEN_COORD_RIGHT(q3 + mb),
							SCREEN_COORD_BOTTOM(q4 + ml), SCREEN_COORD_RIGHT(q5 + mb), SCREEN_COORD_BOTTOM(q6 + ml),
							SCREEN_COORD_RIGHT(q7 + mb), SCREEN_COORD_BOTTOM(q8 + ml), CRGBA(255, 255, 255, 255));
					}
					Events::gameProcessEvent += [&] {
							if(KeyPressed('Q'))
							raiseWatch();

					};
					
				}
			};
		};
		Events::shutdownRwEvent += [] {
			RwTexDictionaryDestroy(watch);
			ht.m_pTexture = nullptr;
			mt.m_pTexture = nullptr;
			bg.m_pTexture = nullptr;
		};
	}
	bool LoadAnimationBlock(const char* szBlockName)
	{
		CAnimBlock* pAnimBlock = CAnimManager::GetAnimationBlock(szBlockName);
		if (pAnimBlock)
		{
			if (!pAnimBlock->bLoaded)
			{
				int animIndex = CAnimManager::GetAnimationBlockIndex(szBlockName);
				CStreaming::RequestModel(25575 + animIndex, MISSION_REQUIRED | PRIORITY_REQUEST);
				pAnimBlock->usRefs++;
				CStreaming::LoadAllRequestedModels(true);
				if (pAnimBlock->bLoaded)
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
		return false;
	}
	void raiseWatch()
	{
				CPlayerPed* pLocalPlayer = FindPlayerPed();
				if (pLocalPlayer)
				{
					const char* szAnimName = "COPLOOK_WATCH";
					const char* szBlockName = "COP_AMBIENT";
					if (LoadAnimationBlock(szBlockName))
					{
						unsigned short animFlags = ANIMATION_UNLOCK_LAST_FRAME | ANIMATION_PARTIAL | ANIMATION_TRANLSATE_X
							| ANIMATION_TRANLSATE_Y;
						CTaskSimpleRunNamedAnim* pTask = new CTaskSimpleRunNamedAnim(
							szAnimName, szBlockName, animFlags, 1.0f, -1, false, true, false, false);

						pLocalPlayer->m_pIntelligence->m_TaskMgr.SetTask(pTask, TASK_PRIMARY_PRIMARY, true);
					}
				}
	}
} watch_SA;
