# DAW Training Studio

A comprehensive web-based Digital Audio Workstation (DAW) training platform built with Web Audio API, designed to teach music production fundamentals through interactive tutorials and hands-on experience.

## Features

### Core DAW Functionality
- **Multi-track Audio Engine**: Create and manage multiple audio tracks with individual controls
- **Transport Controls**: Play, pause, stop, record, and rewind functionality
- **Real-time Audio Processing**: Powered by the Web Audio API for professional-quality audio
- **Waveform Visualization**: Real-time audio visualization to see your sound
- **Tempo Control**: Adjustable BPM (40-240) with precise timing

### Track Management
- Add/remove audio tracks dynamically
- Individual track controls:
  - Volume (gain) control
  - Stereo panning
  - Mute/Solo functionality
  - Record arming
- Visual track representation in timeline

### Audio Effects Suite
- **Reverb**: Simulates room acoustics with adjustable mix and room size
- **Delay**: Configurable echo effect with time and feedback controls
- **3-Band EQ**: Low, mid, and high frequency adjustment
- **Dynamics Compression**: Professional audio dynamics control
- **Effect Presets**: Pre-configured settings for vocals, drums, guitar, bass, and ambient

### Interactive Tutorial System
Five comprehensive tutorials covering:
1. **DAW Basics**: Introduction to digital audio workstations
2. **Working with Tracks**: Track management and organization
3. **Recording Audio**: Capture and record audio effectively
4. **Effects Processing**: Master reverb, delay, EQ, and compression
5. **Mixing Fundamentals**: Create professional-sounding mixes

### Project Management
- Create and save projects
- RESTful API for project persistence
- Track-level settings saved per project
- Effects settings preservation

## Technology Stack

### Frontend
- **Web Audio API**: Core audio processing engine
- **HTML5 Canvas**: Real-time waveform visualization
- **Vanilla JavaScript**: No framework dependencies for maximum compatibility
- **EJS Templates**: Server-side rendering
- **Modern CSS**: Responsive, professional interface

### Backend
- **Node.js**: Server runtime
- **Express.js**: Web server framework
- **RESTful API**: Project management endpoints

## Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd super-spork
```

2. Install dependencies:
```bash
npm install
```

3. Start the development server:
```bash
npm run dev
```

4. Open your browser and navigate to:
```
http://localhost:3000
```

## Usage

### Getting Started
1. Click the "Tutorials" button to learn DAW fundamentals
2. Create a new project using the "New Project" button
3. Add tracks with the "+ Add Track" button
4. Use transport controls to play/pause/stop/record

### Recording Audio
1. Click "+ Add Track" to create a new track
2. Arm the track for recording (R button)
3. Click the record button (red circle) in the transport
4. Grant microphone permissions when prompted
5. Click stop when finished recording

### Applying Effects
1. Select a track by clicking it
2. Use the Effects Panel on the right to:
   - Enable/disable reverb or delay
   - Adjust EQ settings
   - Control master volume

### Saving Your Work
1. Click the "Save" button in the header
2. Your project settings and track configurations will be saved
3. Reload your project anytime using the project API

## API Endpoints

### Projects
- `POST /api/projects` - Create a new project
- `GET /api/projects` - Get all projects
- `GET /api/projects/:id` - Get a specific project
- `PUT /api/projects/:id` - Update a project
- `DELETE /api/projects/:id` - Delete a project

## Architecture

### Audio Engine (`audio-engine.js`)
Manages Web Audio API context, audio tracks, playback, and recording. Handles:
- Audio context initialization
- Track creation and routing
- Buffer management
- Gain and pan controls
- Time synchronization

### Effects Processor (`effects.js`)
Implements audio effects using Web Audio API nodes:
- Convolver-based reverb with impulse response generation
- Delay with feedback loop
- Biquad filter-based 3-band EQ
- Dynamics compressor
- Effect preset system

### Tutorial System (`tutorials.js`)
Comprehensive educational content covering:
- Step-by-step interactive lessons
- Progress tracking
- Rich content with examples and tips
- Navigation between tutorial steps

### DAW Controller (`daw.js`)
Main application controller that:
- Coordinates all subsystems
- Manages UI state
- Handles user interactions
- Updates visualizations
- Integrates project management

## Browser Compatibility

This application requires a modern browser with Web Audio API support:
- Chrome 55+
- Firefox 53+
- Safari 14.1+
- Edge 79+

**Note**: Recording features require HTTPS in production or localhost for development.

## Educational Use

This DAW Training Studio is designed for:
- Music production students learning DAW fundamentals
- Educators teaching audio engineering concepts
- Beginners exploring music production
- Anyone wanting to understand professional audio workflows

## Future Enhancements

Potential features for future development:
- MIDI support and virtual instruments
- Audio file import/export
- Additional effects (chorus, phaser, distortion)
- Automation lanes
- Plugin system
- Collaborative editing
- Cloud storage integration
- More advanced recording features (punch-in/out, comping)

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## License

MIT License - See LICENSE file for details

## Credits

Built with Web Audio API and modern web technologies. Educational content based on industry-standard DAW workflows and best practices.

---

For support or questions, please open an issue on GitHub.