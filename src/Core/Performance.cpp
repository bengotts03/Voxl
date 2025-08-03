//
// Created by Ben Gotts on 03/08/2025.
//

#include "Performance.h"

#include "Time.h"

float Performance::FPS = 0;
float Performance::AverageFPS = 0;

float Performance::MS = 0;

double Performance::_timeDifference = 0;
double Performance::_previousTime = 0;

unsigned int Performance::_fpsCount = 0;

Performance::Performance() = default;

void Performance::Update() {
    _timeDifference = Time::NormalTime - _previousTime;

    _fpsCount++;

    FPS = _timeDifference * _fpsCount;

    if (_timeDifference >= 1.0 / 30.0) {
        AverageFPS = (1.0 / _timeDifference) * _fpsCount;
        MS = (_timeDifference / _fpsCount) * 1000;

        _fpsCount = 0;
        _previousTime = Time::NormalTime;
    }
}
