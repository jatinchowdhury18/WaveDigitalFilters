#pragma once

#include <chowdsp_simd/chowdsp_simd.h>
#include <chowdsp_wdf/chowdsp_wdf.h>

namespace wdft = chowdsp::wdft;

template <typename T>
struct Bucket
{
    using NumericType = chowdsp::NumericType<T>;

    wdft::CapacitorT<T> Cap { (T) (NumericType) 1.0e-7 };
    wdft::ResistiveVoltageSourceT<T> Vin { (T) (NumericType) 100.0 };
    wdft::WDFSeriesT<T, decltype (Cap), decltype (Vin)> S1 { Cap, Vin };
    wdft::SwitchT<T, decltype (S1)> sw1 { S1 };

    void prepare (double sampleRate)
    {
        Cap.prepare ((T) (NumericType) sampleRate);
    }

    void reset()
    {
        Cap.reset();
    }

    inline void setSwitch (bool isSwitchClosed)
    {
        sw1.setClosed (isSwitchClosed);
    }

    inline T process (T x) noexcept
    {
        Vin.setVoltage (x);

        sw1.incident (S1.reflected());
        S1.incident (sw1.reflected());

        return wdft::voltage<T> (Cap);
    }
};

template <typename T>
struct BucketPair
{
    Bucket<T> buckets[2];

    void prepare (double sampleRate)
    {
        for (auto& b : buckets)
            b.prepare (sampleRate);
    }

    void reset()
    {
        for (auto& b : buckets)
            b.reset();
    }

    inline void toggleSwitch (bool switchClosed)
    {
        buckets[0].setSwitch (switchClosed);
        buckets[1].setSwitch (! switchClosed);
    }

    inline T process (T x) noexcept
    {
        return buckets[1].process (buckets[0].process (x));
    }
};

template <int Stages, typename T>
struct BBD
{
    static_assert (std::is_floating_point_v<T>, "T must be a floating point type!");
    static_assert (Stages % 2 == 0, "Number of stages must be even");

    using Vec = xsimd::batch<T>;
    std::array<BucketPair<Vec>, Stages / 2 / Vec::size> buckets;
    T dataRegister alignas (CHOWDSP_WDF_DEFAULT_SIMD_ALIGNMENT)[Vec::size] {};

    static T getClockRateHzForDelayTime (T delayTimeSeconds)
    {
        return T (Stages / 2 / Vec::size) / delayTimeSeconds;
    }

    void prepare (double sampleRate)
    {
        for (auto& b : buckets)
            b.prepare (sampleRate);

        reset();
    }

    void reset()
    {
        for (auto& b : buckets)
            b.reset();

        std::fill (dataRegister, dataRegister + Vec::size, (T) 0);
    }

    inline T process (T x, T clockInput) noexcept
    {
        const auto clockSwitchState = clockInput > (T) 0;

        for (size_t i = 1; i < Vec::size; ++i)
            dataRegister[i] = dataRegister[i - 1];
        dataRegister[0] = x;

        auto simdRegister = xsimd::load_aligned (dataRegister);

        for (auto& b : buckets)
        {
            b.toggleSwitch (clockSwitchState);
            simdRegister = b.process (simdRegister);
        }

        xsimd::store_aligned (dataRegister, simdRegister);

        return dataRegister[Vec::size - 1];
    }
};
