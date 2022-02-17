#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP

#include "main.hpp"

class Animation
{
    function<bool(float)> callback; 
    bool ongoing = true;
public: 
    // callback returns whether the animation is still ongoing
    // and accepts the elapsed time since last frame as a parameter
    Animation(function<bool(float)> a_callback) : callback(a_callback) {}

    Animation() : callback([] (float) {return false;}) {} 

    bool animate(float elapsed_time)
    {
        if (ongoing)
        {
            if (callback(elapsed_time))
                return true;
            else
            {
                ongoing = false;
                return false;
            }
        }
        else return false;
    }

    static
    Animation steps(float step_time, function<bool(void)> callback)
    {
        float *next_step = new float(step_time);
        return (Animation) (function<bool(float)>) // constructor chain using cast operators
        [=] (float elapsed_time)
        {
            *next_step -= elapsed_time;
            if (*next_step <= 0)
            {
                *next_step = step_time;
                
                if (!callback())
                {
                    delete next_step;
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else return true;
        };
    }
};

#endif