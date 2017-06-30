/*
  ==============================================================================

    Vibrato.h
    Created: 30 Jun 2017 4:42:28pm
    Author:  Nikolaj Schwab Andersson
    
  ==============================================================================
    Vibrato plugin optimised with boost.simd

  
*/

#pragma once

#include <boost/simd/function/load.hpp>
#include <boost/simd/function/minus.hpp>
#include <boost/simd/function/store.hpp>
#include <boost/simd/meta/cardinal_of.hpp>
#include <boost/simd/pack.hpp>

class Vibrato{
public:
    float rate;
    float depth;


private:
    float buffer;
    
};
