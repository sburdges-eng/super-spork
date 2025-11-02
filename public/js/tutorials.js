// Tutorial System - Interactive DAW Lessons
class TutorialSystem {
    constructor() {
        this.tutorials = {
            basics: {
                title: 'DAW Basics',
                description: 'Learn the fundamentals of digital audio workstations',
                steps: [
                    {
                        title: 'What is a DAW?',
                        content: `
                            <h3>Welcome to DAW Training!</h3>
                            <p>A Digital Audio Workstation (DAW) is software used for recording, editing,
                            and producing audio files. Think of it as a complete recording studio inside your computer.</p>

                            <h4>Key Components:</h4>
                            <ul>
                                <li><strong>Tracks:</strong> Individual lanes where audio or MIDI data is placed</li>
                                <li><strong>Timeline:</strong> The horizontal representation of time in your project</li>
                                <li><strong>Mixer:</strong> Controls volume, panning, and effects for each track</li>
                                <li><strong>Transport:</strong> Play, pause, stop, and record controls</li>
                            </ul>
                        `
                    },
                    {
                        title: 'Understanding the Interface',
                        content: `
                            <h3>DAW Interface Tour</h3>

                            <h4>Header Section:</h4>
                            <p>At the top, you'll find the transport controls (play, pause, stop, record)
                            and tempo settings. The time display shows your current position in the project.</p>

                            <h4>Track List (Left Sidebar):</h4>
                            <p>This is where all your tracks are listed. Each track can be:</p>
                            <ul>
                                <li>Muted (M button) - Silences the track</li>
                                <li>Soloed (S button) - Only this track plays</li>
                                <li>Armed for recording (R button)</li>
                            </ul>

                            <h4>Timeline (Center):</h4>
                            <p>The main workspace where you arrange your audio clips and see the visual
                            representation of your music over time.</p>

                            <h4>Effects Panel (Right Sidebar):</h4>
                            <p>Control effects like reverb, delay, and EQ to shape your sound.</p>
                        `
                    },
                    {
                        title: 'Basic Workflow',
                        content: `
                            <h3>Typical DAW Workflow</h3>

                            <ol>
                                <li><strong>Create a New Project:</strong> Set your tempo and project name</li>
                                <li><strong>Add Tracks:</strong> Click "+ Add Track" to create new audio tracks</li>
                                <li><strong>Record or Import Audio:</strong> Record directly or load audio files</li>
                                <li><strong>Arrange:</strong> Move and edit clips on the timeline</li>
                                <li><strong>Mix:</strong> Adjust volumes, panning, and add effects</li>
                                <li><strong>Export:</strong> Render your final mix to an audio file</li>
                            </ol>

                            <h4>Try It Yourself:</h4>
                            <p>Click the "+ Add Track" button to create your first track!</p>
                        `
                    }
                ]
            },
            tracks: {
                title: 'Working with Tracks',
                description: 'Add, organize, and manage audio tracks',
                steps: [
                    {
                        title: 'Creating Tracks',
                        content: `
                            <h3>Track Management</h3>

                            <h4>Adding a Track:</h4>
                            <p>Click the "+ Add Track" button in the left sidebar. Each track represents
                            an individual instrument, voice, or sound source in your project.</p>

                            <h4>Track Types:</h4>
                            <ul>
                                <li><strong>Audio Track:</strong> For recording or importing audio files</li>
                                <li><strong>MIDI Track:</strong> For virtual instruments (coming soon)</li>
                                <li><strong>Bus Track:</strong> For grouping and processing multiple tracks</li>
                            </ul>
                        `
                    },
                    {
                        title: 'Track Controls',
                        content: `
                            <h3>Essential Track Controls</h3>

                            <h4>Volume (Gain):</h4>
                            <p>Controls the loudness of the track. Start with all tracks at a moderate
                            level and adjust from there.</p>

                            <h4>Panning:</h4>
                            <p>Positions the sound in the stereo field (left to right). Center (0) is default.</p>

                            <h4>Mute (M):</h4>
                            <p>Temporarily silences the track without deleting it. Useful for A/B comparisons.</p>

                            <h4>Solo (S):</h4>
                            <p>Isolates the track, muting all others. Great for focusing on specific elements.</p>

                            <h4>Record Arm (R):</h4>
                            <p>Enables the track for recording. Only armed tracks will record when you press record.</p>
                        `
                    },
                    {
                        title: 'Organizing Tracks',
                        content: `
                            <h3>Track Organization Tips</h3>

                            <h4>Naming Convention:</h4>
                            <p>Use clear, descriptive names like "Lead Vocal", "Kick Drum", "Bass Guitar"</p>

                            <h4>Common Track Order:</h4>
                            <ol>
                                <li>Drums (Kick, Snare, Hi-Hats, etc.)</li>
                                <li>Bass</li>
                                <li>Rhythm instruments (Guitar, Keys)</li>
                                <li>Lead instruments</li>
                                <li>Vocals (Background, then Lead)</li>
                            </ol>

                            <h4>Color Coding:</h4>
                            <p>Many DAWs allow color coding tracks. Group similar instruments with the same color.</p>
                        `
                    }
                ]
            },
            recording: {
                title: 'Recording Audio',
                description: 'Learn how to record and capture audio',
                steps: [
                    {
                        title: 'Preparing to Record',
                        content: `
                            <h3>Before You Record</h3>

                            <h4>Check Your Input:</h4>
                            <ul>
                                <li>Ensure your microphone or instrument is connected</li>
                                <li>Select the correct input in your audio settings</li>
                                <li>Monitor input levels - aim for -12dB to -6dB peaks</li>
                            </ul>

                            <h4>Set Up Your Track:</h4>
                            <ol>
                                <li>Create a new audio track</li>
                                <li>Arm the track for recording (R button)</li>
                                <li>Enable input monitoring to hear yourself</li>
                                <li>Set an appropriate input gain level</li>
                            </ol>
                        `
                    },
                    {
                        title: 'Recording Techniques',
                        content: `
                            <h3>Recording Best Practices</h3>

                            <h4>The Recording Process:</h4>
                            <ol>
                                <li>Click the record button (red circle)</li>
                                <li>You'll hear a count-in (metronome clicks)</li>
                                <li>Perform your part</li>
                                <li>Click stop when finished</li>
                            </ol>

                            <h4>Tips for Better Recordings:</h4>
                            <ul>
                                <li><strong>Use a metronome:</strong> Keeps your timing consistent</li>
                                <li><strong>Record multiple takes:</strong> You can comp the best parts later</li>
                                <li><strong>Leave headroom:</strong> Don't record too loud - prevents clipping</li>
                                <li><strong>Use a pop filter:</strong> Reduces plosives (p, b, t sounds)</li>
                                <li><strong>Monitor with headphones:</strong> Prevents feedback</li>
                            </ul>
                        `
                    },
                    {
                        title: 'Punch Recording & Overdubs',
                        content: `
                            <h3>Advanced Recording Techniques</h3>

                            <h4>Punch In/Out:</h4>
                            <p>Record over a specific section without affecting the rest of the track.
                            Perfect for fixing mistakes in an otherwise good take.</p>

                            <h4>Overdubbing:</h4>
                            <p>Recording additional parts while listening to existing tracks. This is how
                            you build up a full production one instrument at a time.</p>

                            <h4>Looped Recording:</h4>
                            <p>Record multiple takes over the same section. Great for vocals or solos
                            where you want to capture several variations.</p>

                            <h4>Comping:</h4>
                            <p>Combining the best parts from multiple takes into one perfect performance.</p>
                        `
                    }
                ]
            },
            effects: {
                title: 'Effects Processing',
                description: 'Master reverb, delay, EQ, and compression',
                steps: [
                    {
                        title: 'Understanding Effects',
                        content: `
                            <h3>Audio Effects Basics</h3>

                            <p>Effects are processors that modify your audio signal. They can add space,
                            change tone, control dynamics, or create special effects.</p>

                            <h4>Two Categories of Effects:</h4>

                            <h5>Time-Based Effects (Modulation):</h5>
                            <ul>
                                <li><strong>Reverb:</strong> Simulates room acoustics and space</li>
                                <li><strong>Delay:</strong> Creates echoes and rhythmic repeats</li>
                                <li><strong>Chorus:</strong> Thickens sound by layering slight variations</li>
                            </ul>

                            <h5>Dynamic/Tonal Effects:</h5>
                            <ul>
                                <li><strong>EQ (Equalizer):</strong> Adjusts frequency balance</li>
                                <li><strong>Compression:</strong> Controls dynamic range</li>
                                <li><strong>Distortion:</strong> Adds harmonic saturation</li>
                            </ul>
                        `
                    },
                    {
                        title: 'Reverb & Delay',
                        content: `
                            <h3>Space and Depth</h3>

                            <h4>Reverb:</h4>
                            <p>Reverb simulates the natural reflections in a physical space.</p>
                            <ul>
                                <li><strong>Mix:</strong> How much effect vs. dry signal (start at 20-30%)</li>
                                <li><strong>Room Size:</strong> Larger = longer decay time</li>
                                <li><strong>Use:</strong> Add depth, create atmosphere, blend elements</li>
                            </ul>

                            <h4>Delay:</h4>
                            <p>Delay creates distinct echoes of your audio.</p>
                            <ul>
                                <li><strong>Time:</strong> Delay duration (try 1/4 or 1/8 note timings)</li>
                                <li><strong>Feedback:</strong> Number of repeats (30-40% is typical)</li>
                                <li><strong>Use:</strong> Rhythmic interest, spaciousness, special effects</li>
                            </ul>

                            <h4>Pro Tip:</h4>
                            <p>Use reverb and delay on a send/return bus rather than directly on tracks.
                            This saves CPU and creates a more cohesive sound.</p>
                        `
                    },
                    {
                        title: 'EQ & Compression',
                        content: `
                            <h3>Tone and Dynamics</h3>

                            <h4>EQ (Equalizer):</h4>
                            <p>EQ adjusts the balance of frequencies in your audio.</p>

                            <h5>Frequency Ranges:</h5>
                            <ul>
                                <li><strong>Low (Bass):</strong> 20-320 Hz - Warmth and power</li>
                                <li><strong>Mid:</strong> 320-3200 Hz - Body and presence</li>
                                <li><strong>High (Treble):</strong> 3200-20000 Hz - Clarity and air</li>
                            </ul>

                            <h5>EQ Tips:</h5>
                            <ul>
                                <li>Cut before you boost (remove problems first)</li>
                                <li>Use high-pass filters to remove unnecessary low-end rumble</li>
                                <li>Make small adjustments (1-3 dB often enough)</li>
                            </ul>

                            <h4>Compression:</h4>
                            <p>Compression reduces the dynamic range, making loud parts quieter and
                            allowing you to turn up the overall level.</p>

                            <ul>
                                <li><strong>Threshold:</strong> Level where compression starts (-24 dB typical)</li>
                                <li><strong>Ratio:</strong> How much reduction (4:1 is moderate)</li>
                                <li><strong>Attack:</strong> How fast it responds (fast for drums, slow for vocals)</li>
                                <li><strong>Release:</strong> How quickly it stops compressing</li>
                            </ul>
                        `
                    }
                ]
            },
            mixing: {
                title: 'Mixing Fundamentals',
                description: 'Balance levels, panning, and create a polished mix',
                steps: [
                    {
                        title: 'Mixing Workflow',
                        content: `
                            <h3>The Art of Mixing</h3>

                            <p>Mixing is the process of balancing all your tracks to create a cohesive,
                            professional-sounding final product.</p>

                            <h4>Basic Mixing Steps:</h4>
                            <ol>
                                <li><strong>Gain Staging:</strong> Set initial levels for all tracks</li>
                                <li><strong>Panning:</strong> Position elements in the stereo field</li>
                                <li><strong>EQ:</strong> Carve out frequency space for each element</li>
                                <li><strong>Compression:</strong> Control dynamics</li>
                                <li><strong>Effects:</strong> Add reverb, delay, etc.</li>
                                <li><strong>Automation:</strong> Create movement and interest</li>
                                <li><strong>Reference:</strong> Compare to professional tracks</li>
                            </ol>
                        `
                    },
                    {
                        title: 'Balance and Panning',
                        content: `
                            <h3>Creating Space in Your Mix</h3>

                            <h4>Volume Balance:</h4>
                            <p>Start with faders at unity (0 dB) and adjust from there. Key tips:</p>
                            <ul>
                                <li>Start with drums and bass as your foundation</li>
                                <li>Add other elements one at a time</li>
                                <li>Leave headroom - don't let your master clip!</li>
                                <li>Use volume automation for dynamic parts</li>
                            </ul>

                            <h4>Panning Strategy:</h4>
                            <ul>
                                <li><strong>Center:</strong> Kick, snare, bass, lead vocal</li>
                                <li><strong>Left/Right (slightly):</strong> Toms, guitars, keys</li>
                                <li><strong>Wide:</strong> Background vocals, percussion, effects</li>
                            </ul>

                            <h4>The "LCR" Technique:</h4>
                            <p>Only use Left, Center, and Right positions. This creates clear
                            separation and a more defined stereo image.</p>
                        `
                    },
                    {
                        title: 'Final Polish',
                        content: `
                            <h3>Finishing Your Mix</h3>

                            <h4>Reference Listening:</h4>
                            <ul>
                                <li>Compare your mix to professional tracks in a similar style</li>
                                <li>Listen on multiple systems (headphones, speakers, car, phone)</li>
                                <li>Take breaks - ear fatigue affects judgment</li>
                            </ul>

                            <h4>Common Mistakes to Avoid:</h4>
                            <ul>
                                <li>Mixing too loud - keep your monitor level moderate</li>
                                <li>Over-processing - sometimes less is more</li>
                                <li>Ignoring phase issues - can cause weak bass and thin sound</li>
                                <li>Not using reference tracks</li>
                            </ul>

                            <h4>Before Export:</h4>
                            <ol>
                                <li>Check for clipping (red lights on meters)</li>
                                <li>Ensure proper headroom (-6dB to -3dB on master)</li>
                                <li>Solo check each track for issues</li>
                                <li>Listen start to finish without stopping</li>
                            </ol>
                        `
                    }
                ]
            }
        };

        this.currentTutorial = null;
        this.currentStep = 0;
    }

    getTutorial(id) {
        return this.tutorials[id];
    }

    getAllTutorials() {
        return this.tutorials;
    }

    loadTutorial(id) {
        this.currentTutorial = id;
        this.currentStep = 0;
        return this.renderTutorialContent();
    }

    nextStep() {
        const tutorial = this.tutorials[this.currentTutorial];
        if (this.currentStep < tutorial.steps.length - 1) {
            this.currentStep++;
            return this.renderTutorialContent();
        }
        return null;
    }

    previousStep() {
        if (this.currentStep > 0) {
            this.currentStep--;
            return this.renderTutorialContent();
        }
        return null;
    }

    renderTutorialContent() {
        const tutorial = this.tutorials[this.currentTutorial];
        const step = tutorial.steps[this.currentStep];

        return `
            <div class="tutorial-header">
                <h2>${tutorial.title}</h2>
                <p class="tutorial-description">${tutorial.description}</p>
                <div class="tutorial-progress">
                    Step ${this.currentStep + 1} of ${tutorial.steps.length}
                </div>
            </div>
            <div class="tutorial-content-body">
                <h3>${step.title}</h3>
                ${step.content}
            </div>
            <div class="tutorial-navigation">
                <button id="prev-step" ${this.currentStep === 0 ? 'disabled' : ''}>
                    ← Previous
                </button>
                <button id="next-step" ${this.currentStep === tutorial.steps.length - 1 ? 'disabled' : ''}>
                    Next →
                </button>
                ${this.currentStep === tutorial.steps.length - 1 ?
                    '<button id="complete-tutorial" class="btn-primary">Complete Tutorial</button>' : ''}
            </div>
        `;
    }
}

// Export for use in other modules
if (typeof module !== 'undefined' && module.exports) {
    module.exports = TutorialSystem;
}
