# Quick Start - For Absolute Beginners

**Never built software before? Start here!**

This is the **simplest possible** guide to get the audio player running.

---

## ⚡ Super Quick Version

### Windows Users:

1. **Install Visual Studio 2022** (free)
   - Go to: https://visualstudio.microsoft.com/downloads/
   - Download "Community" edition
   - When installing, check ☑️ "Desktop development with C++"
   - This takes ~30 minutes

2. **Install CMake**
   - Go to: https://cmake.org/download/
   - Download "Windows x64 Installer"
   - When installing, choose "Add CMake to PATH"

3. **Build the app:**
   - Open Command Prompt (press Windows key, type `cmd`)
   - Type these commands (press Enter after each):
   ```
   cd C:\path\to\super-spork\complete-audio-player
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```
   - Wait ~10 minutes (first time only)

4. **Run it:**
   ```
   Release\AudioPlayer.exe
   ```

---

### Mac Users:

1. **Install Xcode** (free)
   - Open App Store
   - Search "Xcode"
   - Click Install (takes ~1 hour, it's 10GB+)

2. **Install Command Line Tools:**
   - Open Terminal (Cmd+Space, type "terminal")
   - Type: `xcode-select --install`
   - Click Install

3. **Install CMake:**
   - Download from: https://cmake.org/download/
   - OR if you have Homebrew: `brew install cmake`

4. **Build the app:**
   - In Terminal, type these commands:
   ```bash
   cd /path/to/super-spork/complete-audio-player
   mkdir build
   cd build
   cmake -G Xcode ..
   cmake --build . --config Release
   ```
   - Wait ~10 minutes (first time only)

5. **Run it:**
   ```bash
   open Release/AudioPlayer.app
   ```

---

### Linux Users (Ubuntu/Debian):

1. **Install everything you need:**
   - Open Terminal (Ctrl+Alt+T)
   - Copy and paste this entire command:
   ```bash
   sudo apt update && sudo apt install -y build-essential cmake git pkg-config libasound2-dev libfreetype6-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libgl1-mesa-dev
   ```
   - Enter your password when asked
   - Wait ~5 minutes

2. **Build the app:**
   ```bash
   cd /path/to/super-spork/complete-audio-player
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
   - Wait ~10 minutes (first time only)

3. **Run it:**
   ```bash
   ./AudioPlayer
   ```

---

## 🎯 What You Should See

When you run `cmake ..`, you should see:
```
-- The C compiler identification is ...
-- The CXX compiler identification is ...
-- Downloading JUCE...
-- Configuring done
-- Generating done
```

When you run `cmake --build ...`, you should see:
```
Building...
[1%] Building CXX object...
[2%] Building CXX object...
...
[100%] Built target AudioPlayer
```

When you run the app, you should see:
- A window titled "JUCE Audio Player"
- Buttons: Load Audio File, Play, Stop, Save Copy As...
- Sliders for Volume and Position
- Labels showing file info

---

## ❌ Common Errors

### "cmake is not recognized" (Windows)
**Fix:**
- Reinstall CMake
- During install, check "Add CMake to system PATH for all users"
- **Restart your computer**
- Try again

### "No CMAKE_CXX_COMPILER could be found"
**Fix (Windows):**
- You didn't install Visual Studio with C++ tools
- Reinstall Visual Studio
- Choose "Desktop development with C++"

**Fix (Mac):**
- Run: `xcode-select --install`

**Fix (Linux):**
- Run: `sudo apt install build-essential`

### "Failed to download JUCE"
**Fix:**
- Check your internet connection
- Wait a few minutes and try again
- If still fails, see README.md for manual JUCE install

### "Could not find JUCE"
**Fix:**
- Make sure you ran `cmake ..` from inside the `build` folder
- Try deleting the `build` folder and starting over

---

## 🆘 Still Stuck?

### Step-by-Step Debugging:

1. **Can you run commands?**
   - Open Terminal/Command Prompt
   - Type: `cmake --version`
   - If you see a version number, CMake is installed ✅
   - If you see "command not found", CMake isn't installed ❌

2. **Are you in the right folder?**
   - Type: `pwd` (Mac/Linux) or `cd` (Windows)
   - You should see a path ending in `complete-audio-player`
   - If not, use `cd` to navigate there

3. **Did the build folder get created?**
   - Type: `ls` (Mac/Linux) or `dir` (Windows)
   - You should see a `build` folder listed
   - If not, run: `mkdir build`

4. **Check the error message**
   - Read the last few lines of red text
   - Google the error message
   - Ask on JUCE forum: forum.juce.com

---

## 🎓 Learning the Commands

Don't worry if these commands seem cryptic! Here's what each one means:

```bash
cd /path/to/folder
```
**"Change Directory"** - Navigate to a folder (like double-clicking a folder in File Explorer)

```bash
mkdir build
```
**"Make Directory"** - Create a new folder called "build"

```bash
cmake ..
```
**"Configure with CMake"** - Read CMakeLists.txt and set up the build
- The `..` means "look in the parent folder for CMakeLists.txt"

```bash
cmake --build . --config Release
```
**"Build the project"** - Compile the code into an executable program
- `--config Release` means "optimized version" (fast, small)

```bash
./AudioPlayer
```
**"Run AudioPlayer"** - Execute the program we just built
- The `./` means "in the current folder"

---

## ⏱️ Time Expectations

**First time building:**
- Installing tools: 30-60 minutes
- Downloading JUCE: 5 minutes
- Building: 5-10 minutes
- **Total: About 1 hour**

**Rebuilding after changes:**
- Just run `cmake --build . --config Release` again
- Takes: 30 seconds - 2 minutes
- Much faster!

---

## ✅ Success Checklist

You've successfully built the app when:
- [ ] No red error messages during build
- [ ] You see "Built target AudioPlayer" at the end
- [ ] The app window opens when you run it
- [ ] You can click buttons in the app
- [ ] You can load and play an audio file

**Got all checkmarks? Congratulations! 🎉**

---

## 🎵 Now What?

### Try These Things:

1. **Load an audio file**
   - Click "Load Audio File"
   - Choose any MP3 or WAV file
   - Watch the info appear

2. **Play with volume**
   - Move the volume slider
   - Hear the difference

3. **Save a file**
   - Load a file
   - Click "Save Copy As..."
   - You just saved a 24-bit WAV!

4. **Change something**
   - Open `MainComponent.cpp` in any text editor
   - Find line 92: `setSize (600, 400);`
   - Change to: `setSize (800, 600);`
   - Rebuild (just run the build command again)
   - The window is now bigger!

---

## 📚 Next Steps After Building

1. ✅ **You built it!** - You've compiled a real audio application
2. 📖 **Read README.md** - More detailed explanations
3. 🎨 **Try customizing** - Change colors, sizes, text
4. 💻 **Learn C++** - Understand the code (see README.md for resources)
5. 🚀 **Build more** - Use the examples in `juce-audio-examples/`

---

## 💡 Understanding What Just Happened

When you ran those commands, here's what actually happened:

1. **CMake downloaded JUCE** (~150 MB of audio code libraries)
2. **CMake generated build files** (instructions for compiling)
3. **The compiler read your code** (Main.cpp, MainComponent.cpp)
4. **It combined your code with JUCE** (mixing your 500 lines with JUCE's millions)
5. **It created an executable** (AudioPlayer.exe or .app)
6. **You ran that executable** - and it works!

**That's software development!** You just did it.

---

**Still having trouble? Drop the exact error message in the JUCE forum and people will help!**

forum.juce.com
