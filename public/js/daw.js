// Main DAW Controller
class DAWController {
    constructor() {
        this.audioEngine = null;
        this.effectsProcessor = null;
        this.tutorialSystem = null;
        this.currentProject = null;
        this.selectedTrack = null;
        this.visualizerActive = false;

        this.init();
    }

    init() {
        // Initialize systems
        this.audioEngine = new AudioEngine();
        this.effectsProcessor = new EffectsProcessor(
            this.audioEngine.audioContext,
            this.audioEngine.masterGain
        );
        this.tutorialSystem = new TutorialSystem();

        // Connect effects chain
        this.effectsProcessor.connectEffectsChain(
            this.audioEngine.masterGain,
            this.audioEngine.masterGain
        );

        // Setup event listeners
        this.setupEventListeners();

        // Initialize visualizer
        this.initVisualizer();

        // Create default project
        this.createNewProject();

        console.log('DAW Controller initialized');
    }

    setupEventListeners() {
        // Transport controls
        document.getElementById('play-btn').addEventListener('click', () => this.play());
        document.getElementById('pause-btn').addEventListener('click', () => this.pause());
        document.getElementById('stop-btn').addEventListener('click', () => this.stop());
        document.getElementById('record-btn').addEventListener('click', () => this.toggleRecord());
        document.getElementById('rewind-btn').addEventListener('click', () => this.rewind());

        // Tempo control
        document.getElementById('tempo-input').addEventListener('change', (e) => {
            this.setTempo(parseInt(e.target.value));
        });

        // Track management
        document.getElementById('add-track-btn').addEventListener('click', () => this.addTrack());

        // Project management
        document.getElementById('new-project-btn').addEventListener('click', () => this.createNewProject());
        document.getElementById('save-btn').addEventListener('click', () => this.saveProject());

        // Tutorial
        document.getElementById('tutorial-btn').addEventListener('click', () => this.openTutorials());

        // Master volume
        document.getElementById('master-volume').addEventListener('input', (e) => {
            const value = parseInt(e.target.value) / 100;
            this.audioEngine.setMasterVolume(value);
            document.getElementById('master-volume-value').textContent = e.target.value + '%';
        });

        // Effects controls
        this.setupEffectsListeners();

        // Modal close buttons
        document.querySelectorAll('.close').forEach(btn => {
            btn.addEventListener('click', (e) => {
                e.target.closest('.modal').classList.add('hidden');
            });
        });

        // Tutorial items
        document.querySelectorAll('.tutorial-item').forEach(item => {
            item.addEventListener('click', () => {
                const tutorialId = item.getAttribute('data-tutorial');
                this.loadTutorial(tutorialId);
            });
        });

        // Set recording complete handler
        this.audioEngine.onRecordingComplete = (blob) => {
            this.handleRecordingComplete(blob);
        };
    }

    setupEffectsListeners() {
        // Reverb
        document.getElementById('reverb-toggle').addEventListener('change', (e) => {
            this.effectsProcessor.enableReverb(e.target.checked);
        });

        document.getElementById('reverb-mix').addEventListener('input', (e) => {
            this.effectsProcessor.setReverbMix(parseInt(e.target.value) / 100);
        });

        document.getElementById('reverb-room').addEventListener('input', (e) => {
            this.effectsProcessor.setReverbRoom(parseInt(e.target.value) / 100);
        });

        // Delay
        document.getElementById('delay-toggle').addEventListener('change', (e) => {
            this.effectsProcessor.enableDelay(e.target.checked);
        });

        document.getElementById('delay-time').addEventListener('input', (e) => {
            this.effectsProcessor.setDelayTime(parseInt(e.target.value));
        });

        document.getElementById('delay-feedback').addEventListener('input', (e) => {
            this.effectsProcessor.setDelayFeedback(parseInt(e.target.value));
        });

        // EQ
        document.getElementById('eq-low').addEventListener('input', (e) => {
            this.effectsProcessor.setEQLow(parseInt(e.target.value));
        });

        document.getElementById('eq-mid').addEventListener('input', (e) => {
            this.effectsProcessor.setEQMid(parseInt(e.target.value));
        });

        document.getElementById('eq-high').addEventListener('input', (e) => {
            this.effectsProcessor.setEQHigh(parseInt(e.target.value));
        });
    }

    // Transport controls
    play() {
        this.audioEngine.play();
        document.getElementById('play-btn').classList.add('active');
        document.getElementById('pause-btn').classList.remove('active');
        this.startVisualizer();
    }

    pause() {
        this.audioEngine.pause();
        document.getElementById('play-btn').classList.remove('active');
        document.getElementById('pause-btn').classList.add('active');
    }

    stop() {
        this.audioEngine.stop();
        document.getElementById('play-btn').classList.remove('active');
        document.getElementById('pause-btn').classList.remove('active');
        this.updateTimeDisplay(0);
    }

    rewind() {
        this.stop();
    }

    toggleRecord() {
        const recordBtn = document.getElementById('record-btn');

        if (!this.audioEngine.isRecording) {
            this.audioEngine.record();
            recordBtn.classList.add('active');
        } else {
            this.audioEngine.stopRecording();
            recordBtn.classList.remove('active');
        }
    }

    handleRecordingComplete(blob) {
        // Create a temporary audio element to preview
        const url = URL.createObjectURL(blob);
        console.log('Recording saved:', url);

        // In a real app, you'd add this to a track
        alert('Recording complete! In a full version, this would be added to a track.');
    }

    setTempo(bpm) {
        this.audioEngine.setTempo(bpm);
        if (this.currentProject) {
            this.currentProject.tempo = bpm;
        }
    }

    // Track management
    addTrack() {
        const trackCount = this.audioEngine.tracks.length;
        const trackName = `Track ${trackCount + 1}`;
        const track = this.audioEngine.createTrack(trackName);

        this.renderTrack(track);
        this.addTrackToTimeline(track);

        // Add a test tone to demonstrate
        if (trackCount === 0) {
            const testBuffer = this.audioEngine.generateTestTone(440, 2.0);
            track.clips.push({
                id: Date.now(),
                name: 'Test Tone',
                buffer: testBuffer,
                startTime: 0,
                duration: 2.0,
                offset: 0
            });
        }
    }

    renderTrack(track) {
        const trackList = document.getElementById('track-list');

        const trackElement = document.createElement('div');
        trackElement.className = 'track-item';
        trackElement.setAttribute('data-track-id', track.id);

        trackElement.innerHTML = `
            <div class="track-name">${track.name}</div>
            <div class="track-controls">
                <button class="track-mute" title="Mute">M</button>
                <button class="track-solo" title="Solo">S</button>
                <button class="track-record" title="Record">R</button>
            </div>
            <label>Vol: <input type="range" class="track-volume" min="0" max="100" value="80"></label>
            <label>Pan: <input type="range" class="track-pan" min="-100" max="100" value="0"></label>
        `;

        // Add event listeners
        trackElement.querySelector('.track-mute').addEventListener('click', () => {
            this.audioEngine.muteTrack(track.id);
            trackElement.querySelector('.track-mute').classList.toggle('active');
        });

        trackElement.querySelector('.track-solo').addEventListener('click', () => {
            this.audioEngine.soloTrack(track.id);
            trackElement.querySelector('.track-solo').classList.toggle('active');
        });

        trackElement.querySelector('.track-volume').addEventListener('input', (e) => {
            this.audioEngine.setTrackVolume(track.id, parseInt(e.target.value) / 100);
        });

        trackElement.querySelector('.track-pan').addEventListener('input', (e) => {
            this.audioEngine.setTrackPan(track.id, parseInt(e.target.value) / 100);
        });

        trackElement.addEventListener('click', (e) => {
            if (!e.target.closest('button, input')) {
                this.selectTrack(track.id);
            }
        });

        trackList.appendChild(trackElement);
    }

    addTrackToTimeline(track) {
        const timeline = document.getElementById('timeline-tracks');

        const trackLane = document.createElement('div');
        trackLane.className = 'track-lane';
        trackLane.setAttribute('data-track-id', track.id);

        // Add clips if any
        track.clips.forEach(clip => {
            const clipElement = this.createClipElement(clip);
            trackLane.appendChild(clipElement);
        });

        timeline.appendChild(trackLane);
    }

    createClipElement(clip) {
        const clipElement = document.createElement('div');
        clipElement.className = 'audio-clip';
        clipElement.style.left = (clip.startTime * 100) + 'px';
        clipElement.style.width = (clip.duration * 100) + 'px';
        clipElement.textContent = clip.name;
        clipElement.setAttribute('data-clip-id', clip.id);

        return clipElement;
    }

    selectTrack(trackId) {
        // Remove previous selection
        document.querySelectorAll('.track-item').forEach(el => {
            el.classList.remove('selected');
        });

        // Add selection
        const trackElement = document.querySelector(`[data-track-id="${trackId}"]`);
        if (trackElement) {
            trackElement.classList.add('selected');
            this.selectedTrack = trackId;
        }
    }

    // Project management
    createNewProject() {
        const projectName = prompt('Enter project name:', 'My Project');
        if (!projectName) return;

        fetch('/api/projects', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ name: projectName })
        })
        .then(res => res.json())
        .then(project => {
            this.currentProject = project;
            document.getElementById('project-name').textContent = project.name;
            console.log('Project created:', project);
        })
        .catch(err => console.error('Error creating project:', err));
    }

    saveProject() {
        if (!this.currentProject) {
            alert('No project to save');
            return;
        }

        // Gather project data
        const projectData = {
            ...this.currentProject,
            tracks: this.audioEngine.tracks.map(track => ({
                id: track.id,
                name: track.name,
                volume: track.volume,
                pan: track.pan,
                muted: track.muted,
                solo: track.solo
            })),
            effects: this.effectsProcessor.getEffectSettings(),
            tempo: this.audioEngine.tempo
        };

        fetch(`/api/projects/${this.currentProject.id}`, {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(projectData)
        })
        .then(res => res.json())
        .then(project => {
            console.log('Project saved:', project);
            alert('Project saved successfully!');
        })
        .catch(err => {
            console.error('Error saving project:', err);
            alert('Error saving project');
        });
    }

    // Tutorial system
    openTutorials() {
        document.getElementById('tutorial-modal').classList.remove('hidden');
    }

    loadTutorial(tutorialId) {
        const content = this.tutorialSystem.loadTutorial(tutorialId);
        const contentModal = document.getElementById('tutorial-content-modal');
        const contentDiv = document.getElementById('tutorial-content');

        contentDiv.innerHTML = content;

        // Hide tutorial list modal
        document.getElementById('tutorial-modal').classList.add('hidden');

        // Show content modal
        contentModal.classList.remove('hidden');

        // Setup navigation
        this.setupTutorialNavigation();
    }

    setupTutorialNavigation() {
        const prevBtn = document.getElementById('prev-step');
        const nextBtn = document.getElementById('next-step');
        const completeBtn = document.getElementById('complete-tutorial');

        if (prevBtn) {
            prevBtn.addEventListener('click', () => {
                const content = this.tutorialSystem.previousStep();
                if (content) {
                    document.getElementById('tutorial-content').innerHTML = content;
                    this.setupTutorialNavigation();
                }
            });
        }

        if (nextBtn) {
            nextBtn.addEventListener('click', () => {
                const content = this.tutorialSystem.nextStep();
                if (content) {
                    document.getElementById('tutorial-content').innerHTML = content;
                    this.setupTutorialNavigation();
                }
            });
        }

        if (completeBtn) {
            completeBtn.addEventListener('click', () => {
                document.getElementById('tutorial-content-modal').classList.add('hidden');
                alert('Tutorial complete! Great job!');
            });
        }
    }

    // Visualizer
    initVisualizer() {
        this.canvas = document.getElementById('waveform-canvas');
        this.canvasCtx = this.canvas.getContext('2d');

        // Set canvas size
        this.canvas.width = this.canvas.offsetWidth;
        this.canvas.height = this.canvas.offsetHeight;
    }

    startVisualizer() {
        if (this.visualizerActive) return;
        this.visualizerActive = true;
        this.drawWaveform();
    }

    drawWaveform() {
        if (!this.visualizerActive) return;

        const dataArray = this.audioEngine.getAnalyserData();
        const bufferLength = dataArray.length;

        this.canvasCtx.fillStyle = '#1a1a1a';
        this.canvasCtx.fillRect(0, 0, this.canvas.width, this.canvas.height);

        this.canvasCtx.lineWidth = 2;
        this.canvasCtx.strokeStyle = '#3498db';
        this.canvasCtx.beginPath();

        const sliceWidth = this.canvas.width / bufferLength;
        let x = 0;

        for (let i = 0; i < bufferLength; i++) {
            const v = dataArray[i] / 128.0;
            const y = v * this.canvas.height / 2;

            if (i === 0) {
                this.canvasCtx.moveTo(x, y);
            } else {
                this.canvasCtx.lineTo(x, y);
            }

            x += sliceWidth;
        }

        this.canvasCtx.lineTo(this.canvas.width, this.canvas.height / 2);
        this.canvasCtx.stroke();

        // Update time display
        this.updateTimeDisplay(this.audioEngine.getCurrentTime());

        requestAnimationFrame(() => this.drawWaveform());
    }

    updateTimeDisplay(seconds) {
        const minutes = Math.floor(seconds / 60);
        const secs = Math.floor(seconds % 60);
        const ms = Math.floor((seconds % 1) * 1000);

        const display = `${minutes.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}:${ms.toString().padStart(3, '0')}`;
        document.getElementById('time-display').textContent = display;
    }
}

// Initialize when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    window.daw = new DAWController();
});
