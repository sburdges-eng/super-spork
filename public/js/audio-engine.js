// Audio Engine - Web Audio API Implementation
class AudioEngine {
    constructor() {
        this.audioContext = null;
        this.tracks = [];
        this.masterGain = null;
        this.analyser = null;
        this.isPlaying = false;
        this.isPaused = false;
        this.isRecording = false;
        this.startTime = 0;
        this.currentTime = 0;
        this.tempo = 120;
        this.animationId = null;

        this.init();
    }

    init() {
        // Create AudioContext
        const AudioContext = window.AudioContext || window.webkitAudioContext;
        this.audioContext = new AudioContext();

        // Create master gain node
        this.masterGain = this.audioContext.createGain();
        this.masterGain.gain.value = 0.8;

        // Create analyser for visualization
        this.analyser = this.audioContext.createAnalyser();
        this.analyser.fftSize = 2048;

        // Connect master chain
        this.masterGain.connect(this.analyser);
        this.analyser.connect(this.audioContext.destination);

        console.log('Audio Engine initialized');
    }

    createTrack(name, type = 'audio') {
        const track = {
            id: Date.now() + Math.random(),
            name: name,
            type: type,
            clips: [],
            gainNode: this.audioContext.createGain(),
            panNode: this.audioContext.createStereoPanner(),
            muted: false,
            solo: false,
            volume: 0.8,
            pan: 0,
            effects: []
        };

        // Connect track chain
        track.gainNode.connect(track.panNode);
        track.panNode.connect(this.masterGain);

        this.tracks.push(track);
        return track;
    }

    removeTrack(trackId) {
        const index = this.tracks.findIndex(t => t.id === trackId);
        if (index !== -1) {
            const track = this.tracks[index];
            // Disconnect and cleanup
            track.gainNode.disconnect();
            track.panNode.disconnect();
            this.tracks.splice(index, 1);
        }
    }

    loadAudioFile(file, track) {
        return new Promise((resolve, reject) => {
            const reader = new FileReader();

            reader.onload = (e) => {
                this.audioContext.decodeAudioData(e.target.result)
                    .then(audioBuffer => {
                        const clip = {
                            id: Date.now() + Math.random(),
                            name: file.name,
                            buffer: audioBuffer,
                            startTime: 0,
                            duration: audioBuffer.duration,
                            offset: 0
                        };

                        track.clips.push(clip);
                        resolve(clip);
                    })
                    .catch(error => reject(error));
            };

            reader.onerror = reject;
            reader.readAsArrayBuffer(file);
        });
    }

    play() {
        if (this.audioContext.state === 'suspended') {
            this.audioContext.resume();
        }

        if (this.isPaused) {
            // Resume from pause
            this.isPaused = false;
        } else {
            // Start from beginning or current position
            this.startTime = this.audioContext.currentTime - this.currentTime;
        }

        this.isPlaying = true;

        // Play all clips on all tracks
        this.tracks.forEach(track => {
            if (!track.muted) {
                track.clips.forEach(clip => {
                    this.playClip(clip, track);
                });
            }
        });

        // Start time update
        this.updateTime();
    }

    playClip(clip, track) {
        const source = this.audioContext.createBufferSource();
        source.buffer = clip.buffer;

        // Connect to track's gain node
        source.connect(track.gainNode);

        // Schedule playback
        const when = this.startTime + clip.startTime;
        const offset = clip.offset;
        const duration = clip.duration;

        source.start(when, offset, duration);

        // Store reference for stopping
        clip.source = source;
    }

    pause() {
        this.isPaused = true;
        this.isPlaying = false;

        // Stop all playing sources
        this.stopAllSources();

        // Cancel animation
        if (this.animationId) {
            cancelAnimationFrame(this.animationId);
        }
    }

    stop() {
        this.isPlaying = false;
        this.isPaused = false;
        this.currentTime = 0;

        this.stopAllSources();

        // Cancel animation
        if (this.animationId) {
            cancelAnimationFrame(this.animationId);
        }
    }

    stopAllSources() {
        this.tracks.forEach(track => {
            track.clips.forEach(clip => {
                if (clip.source) {
                    try {
                        clip.source.stop();
                    } catch (e) {
                        // Source may already be stopped
                    }
                    clip.source = null;
                }
            });
        });
    }

    record() {
        if (!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia) {
            alert('Recording not supported in this browser');
            return;
        }

        this.isRecording = true;

        navigator.mediaDevices.getUserMedia({ audio: true })
            .then(stream => {
                this.mediaRecorder = new MediaRecorder(stream);
                this.recordedChunks = [];

                this.mediaRecorder.ondataavailable = (e) => {
                    if (e.data.size > 0) {
                        this.recordedChunks.push(e.data);
                    }
                };

                this.mediaRecorder.onstop = () => {
                    const blob = new Blob(this.recordedChunks, { type: 'audio/webm' });
                    this.onRecordingComplete(blob);
                    stream.getTracks().forEach(track => track.stop());
                };

                this.mediaRecorder.start();
                console.log('Recording started');
            })
            .catch(error => {
                console.error('Error accessing microphone:', error);
                alert('Could not access microphone');
                this.isRecording = false;
            });
    }

    stopRecording() {
        if (this.mediaRecorder && this.isRecording) {
            this.mediaRecorder.stop();
            this.isRecording = false;
            console.log('Recording stopped');
        }
    }

    onRecordingComplete(blob) {
        // This will be overridden by the main DAW controller
        console.log('Recording complete', blob);
    }

    setMasterVolume(value) {
        this.masterGain.gain.value = value;
    }

    setTrackVolume(trackId, value) {
        const track = this.tracks.find(t => t.id === trackId);
        if (track) {
            track.volume = value;
            track.gainNode.gain.value = value;
        }
    }

    setTrackPan(trackId, value) {
        const track = this.tracks.find(t => t.id === trackId);
        if (track) {
            track.pan = value;
            track.panNode.pan.value = value;
        }
    }

    muteTrack(trackId) {
        const track = this.tracks.find(t => t.id === trackId);
        if (track) {
            track.muted = !track.muted;
            track.gainNode.gain.value = track.muted ? 0 : track.volume;
        }
    }

    soloTrack(trackId) {
        const track = this.tracks.find(t => t.id === trackId);
        if (track) {
            track.solo = !track.solo;

            // If any track is soloed, mute all non-solo tracks
            const hasSolo = this.tracks.some(t => t.solo);

            this.tracks.forEach(t => {
                if (hasSolo) {
                    t.gainNode.gain.value = t.solo ? t.volume : 0;
                } else {
                    t.gainNode.gain.value = t.muted ? 0 : t.volume;
                }
            });
        }
    }

    setTempo(bpm) {
        this.tempo = bpm;
    }

    updateTime() {
        if (this.isPlaying) {
            this.currentTime = this.audioContext.currentTime - this.startTime;
            this.animationId = requestAnimationFrame(() => this.updateTime());
        }
    }

    getCurrentTime() {
        return this.currentTime;
    }

    getAnalyserData() {
        const bufferLength = this.analyser.frequencyBinCount;
        const dataArray = new Uint8Array(bufferLength);
        this.analyser.getByteTimeDomainData(dataArray);
        return dataArray;
    }

    // Generate a simple test tone
    generateTestTone(frequency = 440, duration = 1.0) {
        const sampleRate = this.audioContext.sampleRate;
        const buffer = this.audioContext.createBuffer(1, sampleRate * duration, sampleRate);
        const data = buffer.getChannelData(0);

        for (let i = 0; i < buffer.length; i++) {
            data[i] = Math.sin(2 * Math.PI * frequency * i / sampleRate) * 0.3;
        }

        return buffer;
    }
}

// Export for use in other modules
if (typeof module !== 'undefined' && module.exports) {
    module.exports = AudioEngine;
}
