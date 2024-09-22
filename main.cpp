#include <atomic>
#include <thread>
#include <SonicCore_C/SonicCore_C.h>

struct AppState
{
    std::atomic_bool shouldExit{ 0 };
    SCAudioIOBox ioBox;
    SCMT19937 mt19937;
    SCBiquad biquad;
};

static inline uint32_t ioCallback(struct __SCAudioIOBox_S* device,
                                  SCAudioHALIOCallbackContext* context,
                                  void* userData) noexcept
{
    static float fBuf[1024]{};
    static uint32_t gU32Buf[1024]{};
    AppState* state = reinterpret_cast<AppState*>(userData);
    float* outBuff =
        reinterpret_cast<float*>(context->outputBuffers.buffers[0].bytes);
    for (uint32_t i = 0; i < context->blockSize; ++i)
    {
        gU32Buf[i] = SCMT19937_Gen_U32(&state->mt19937);
    }
    SCTypeConversion_U32ToFloat(fBuf, gU32Buf, context->blockSize);
    SCBiquad_Process(fBuf, fBuf, context->blockSize, &state->biquad);
    for (uint32_t sample = 0; sample < context->blockSize; ++sample)
    {
        for (uint32_t ch = 0; ch < context->outputFormat.numChannels; ++ch)
        {
            *(outBuff++) = fBuf[sample];
        }
    }
}

static inline uint32_t prepareForIO(struct __SCAudioIOBox_S* device,
                                    const SCAudioHALIOCallbackContext* context,
                                    void* userData) noexcept
{
    AppState* state = reinterpret_cast<AppState*>(userData);
    SCMT19937_Init(&state->mt19937, 1237);
    state->biquad.coeff =
        SCFilterDesign_Biquad_LPF(context->outputFormat.sampleRate, 200, 0.707);
    SCBiquad_Prepare(&state->biquad);
}

static inline uint32_t ioStopped(struct __SCAudioIOBox_S* device,
                                 void* userData) noexcept
{
}

int main()
{
    AppState appState;

    SCAudioHAL_Init(eSCOSAudioDriverType_CoreAudio);
    appState.ioBox =
        SCAudioHAL_GetDefaultOutputDevice(eSCOSAudioDriverType_CoreAudio);
    SCAudioHALIOHandler halIOHandler{ prepareForIO,
                                      ioCallback,
                                      ioStopped,
                                      &appState };
    SCAudioIOBox_AddIOHandler(&appState.ioBox, &halIOHandler);
    SCAudioStreamFormat fmt;
    fmt.bitsPerSample   = 32;
    fmt.numChannels     = 1;
    fmt.bytesPerFrame   = 4;
    fmt.formatTypeID    = eSCAudioStreamFormatDataType_PCM;
    fmt.sampleRate      = 48000;
    uint32_t bufferSize = 512;
    SCAudioIOBox_PrepareForIO(&appState.ioBox, &fmt, &bufferSize);
    SCAudioIOBox_StartIO(&appState.ioBox);

    while (!appState.shouldExit.load(std::memory_order_relaxed))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }

    SCAudioIOBox_StopIO(&appState.ioBox);
    SCAudioHAL_Deinit(eSCOSAudioDriverType_CoreAudio);
    return 0;
}
