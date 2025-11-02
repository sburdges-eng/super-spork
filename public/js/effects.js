// Effects Processor - Audio Effects Implementation
class EffectsProcessor {
    constructor(audioContext, masterGain) {
        this.audioContext = audioContext;
        this.masterGain = masterGain;

        // Effect nodes
        this.reverbNode = null;
        this.reverbGain = null;
        this.delayNode = null;
        this.delayFeedback = null;
        this.delayGain = null;
        this.eqLow = null;
        this.eqMid = null;
        this.eqHigh = null;
        this.compressor = null;

        // Effect states
        this.effects = {
            reverb: { enabled: false, mix: 0.3, room: 0.5 },
            delay: { enabled: false, time: 0.25, feedback: 0.3 },
            eq: { low: 0, mid: 0, high: 0 },
            compressor: { enabled: false, threshold: -24, ratio: 12 }
        };

        this.init();
    }

    init() {
        this.setupReverb();
        this.setupDelay();
        this.setupEQ();
        this.setupCompressor();
        console.log('Effects processor initialized');
    }

    setupReverb() {
        // Create convolver for reverb
        this.reverbNode = this.audioContext.createConvolver();
        this.reverbGain = this.audioContext.createGain();
        this.reverbGain.gain.value = 0;

        // Generate impulse response
        this.generateReverbImpulse(2, 2);

        // Connect: input -> reverb -> gain -> master
        this.reverbNode.connect(this.reverbGain);
    }

    generateReverbImpulse(duration, decay) {
        const sampleRate = this.audioContext.sampleRate;
        const length = sampleRate * duration;
        const impulse = this.audioContext.createBuffer(2, length, sampleRate);
        const impulseL = impulse.getChannelData(0);
        const impulseR = impulse.getChannelData(1);

        for (let i = 0; i < length; i++) {
            const n = length - i;
            impulseL[i] = (Math.random() * 2 - 1) * Math.pow(n / length, decay);
            impulseR[i] = (Math.random() * 2 - 1) * Math.pow(n / length, decay);
        }

        this.reverbNode.buffer = impulse;
    }

    setupDelay() {
        // Create delay node
        this.delayNode = this.audioContext.createDelay(5.0);
        this.delayNode.delayTime.value = 0.25;

        // Create feedback path
        this.delayFeedback = this.audioContext.createGain();
        this.delayFeedback.gain.value = 0.3;

        // Create wet/dry mix
        this.delayGain = this.audioContext.createGain();
        this.delayGain.gain.value = 0;

        // Connect: input -> delay -> feedback -> delay (loop)
        this.delayNode.connect(this.delayFeedback);
        this.delayFeedback.connect(this.delayNode);
        this.delayNode.connect(this.delayGain);
    }

    setupEQ() {
        // Create three-band EQ using biquad filters
        this.eqLow = this.audioContext.createBiquadFilter();
        this.eqLow.type = 'lowshelf';
        this.eqLow.frequency.value = 320;
        this.eqLow.gain.value = 0;

        this.eqMid = this.audioContext.createBiquadFilter();
        this.eqMid.type = 'peaking';
        this.eqMid.frequency.value = 1000;
        this.eqMid.Q.value = 0.5;
        this.eqMid.gain.value = 0;

        this.eqHigh = this.audioContext.createBiquadFilter();
        this.eqHigh.type = 'highshelf';
        this.eqHigh.frequency.value = 3200;
        this.eqHigh.gain.value = 0;

        // Chain EQ nodes
        this.eqLow.connect(this.eqMid);
        this.eqMid.connect(this.eqHigh);
    }

    setupCompressor() {
        // Create dynamics compressor
        this.compressor = this.audioContext.createDynamicsCompressor();
        this.compressor.threshold.value = -24;
        this.compressor.knee.value = 30;
        this.compressor.ratio.value = 12;
        this.compressor.attack.value = 0.003;
        this.compressor.release.value = 0.25;
    }

    connectEffectsChain(inputNode, outputNode) {
        // Dry signal path
        inputNode.connect(this.eqLow);
        this.eqHigh.connect(this.compressor);
        this.compressor.connect(outputNode);

        // Wet signal paths (parallel)
        inputNode.connect(this.reverbNode);
        this.reverbGain.connect(outputNode);

        inputNode.connect(this.delayNode);
        this.delayGain.connect(outputNode);
    }

    // Reverb controls
    enableReverb(enabled) {
        this.effects.reverb.enabled = enabled;
        this.reverbGain.gain.setTargetAtTime(
            enabled ? this.effects.reverb.mix : 0,
            this.audioContext.currentTime,
            0.1
        );
    }

    setReverbMix(value) {
        this.effects.reverb.mix = value;
        if (this.effects.reverb.enabled) {
            this.reverbGain.gain.setTargetAtTime(
                value,
                this.audioContext.currentTime,
                0.1
            );
        }
    }

    setReverbRoom(value) {
        this.effects.reverb.room = value;
        // Regenerate impulse with new decay
        const duration = 1 + value * 3; // 1-4 seconds
        const decay = 2 + value * 4; // 2-6 decay
        this.generateReverbImpulse(duration, decay);
    }

    // Delay controls
    enableDelay(enabled) {
        this.effects.delay.enabled = enabled;
        this.delayGain.gain.setTargetAtTime(
            enabled ? 0.5 : 0,
            this.audioContext.currentTime,
            0.1
        );
    }

    setDelayTime(value) {
        this.effects.delay.time = value / 1000; // Convert ms to seconds
        this.delayNode.delayTime.setTargetAtTime(
            this.effects.delay.time,
            this.audioContext.currentTime,
            0.1
        );
    }

    setDelayFeedback(value) {
        this.effects.delay.feedback = value / 100;
        this.delayFeedback.gain.setTargetAtTime(
            this.effects.delay.feedback,
            this.audioContext.currentTime,
            0.1
        );
    }

    // EQ controls
    setEQLow(value) {
        this.effects.eq.low = value;
        this.eqLow.gain.setTargetAtTime(
            value,
            this.audioContext.currentTime,
            0.1
        );
    }

    setEQMid(value) {
        this.effects.eq.mid = value;
        this.eqMid.gain.setTargetAtTime(
            value,
            this.audioContext.currentTime,
            0.1
        );
    }

    setEQHigh(value) {
        this.effects.eq.high = value;
        this.eqHigh.gain.setTargetAtTime(
            value,
            this.audioContext.currentTime,
            0.1
        );
    }

    // Compressor controls
    enableCompressor(enabled) {
        this.effects.compressor.enabled = enabled;
        // Compressor is always in the chain, just adjust threshold
        this.compressor.threshold.value = enabled ? -24 : 0;
    }

    setCompressorThreshold(value) {
        this.effects.compressor.threshold = value;
        this.compressor.threshold.value = value;
    }

    setCompressorRatio(value) {
        this.effects.compressor.ratio = value;
        this.compressor.ratio.value = value;
    }

    // Get current effect settings
    getEffectSettings() {
        return this.effects;
    }

    // Load effect preset
    loadPreset(preset) {
        switch(preset) {
            case 'vocal':
                this.setEQLow(2);
                this.setEQMid(3);
                this.setEQHigh(4);
                this.enableReverb(true);
                this.setReverbMix(0.25);
                this.enableCompressor(true);
                break;
            case 'drums':
                this.setEQLow(4);
                this.setEQMid(-2);
                this.setEQHigh(2);
                this.enableCompressor(true);
                this.setCompressorRatio(8);
                break;
            case 'guitar':
                this.setEQLow(0);
                this.setEQMid(2);
                this.setEQHigh(0);
                this.enableDelay(true);
                this.setDelayTime(375);
                this.setDelayFeedback(30);
                break;
            case 'bass':
                this.setEQLow(6);
                this.setEQMid(-3);
                this.setEQHigh(-6);
                this.enableCompressor(true);
                break;
            case 'ambient':
                this.enableReverb(true);
                this.setReverbMix(0.6);
                this.setReverbRoom(0.8);
                this.enableDelay(true);
                this.setDelayTime(500);
                this.setDelayFeedback(45);
                break;
            default:
                // Reset all
                this.setEQLow(0);
                this.setEQMid(0);
                this.setEQHigh(0);
                this.enableReverb(false);
                this.enableDelay(false);
                this.enableCompressor(false);
        }
    }
}

// Export for use in other modules
if (typeof module !== 'undefined' && module.exports) {
    module.exports = EffectsProcessor;
}
