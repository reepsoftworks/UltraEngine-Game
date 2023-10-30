//========= Copyright Reep Softworks, All rights reserved. ============//
// 
// https://github.com/reepsoftworks/UltraEngine-Game
// This code is free to use for those who wish to develop and ship their 
// commerical or non-commerical game projects using Ultra Engine.
// Do not remove this notice.
// 
//=====================================================================//
#include "UltraEngine.h"
#include "RandomNumber.h"

namespace UltraEngine::Game
{
	int RandomNumber(const int min, const int max)
	{
        static bool first = true;
        if (first)
        {
            int t = (unsigned int)time(NULL);
            srand(t); //seeding for the first time only!
            first = false;
        }

        auto range = max - min + 1;
        auto num = rand() % range + min;
        return static_cast<int>(num);
	}

    float RandomFloat(const float min, const float max)
    {
        static bool first = true;
        if (first)
        {
            int t = (unsigned int)time(NULL);
            srand(t); //seeding for the first time only!
            first = false;
        }

        auto range = max - min + 1;
        auto num = rand() % (int)range + min;
        return static_cast<float>(num);
    }

    bool RandomState()
    {
        static bool first = true;
        if (first)
        {
            int t = (unsigned int)time(NULL);
            srand(t); //seeding for the first time only!
            first = false;
        }

        auto min = 0;
        auto max = 1;
        auto range = max - min + 1;
        auto num = rand() % range + min;
        return static_cast<bool>(num);
    }
}