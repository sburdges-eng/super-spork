#!/bin/bash

# JUCE Audio Player - Build Script for macOS/Linux
# This script automates the build process

echo "================================================"
echo "  JUCE Audio Player - Automated Build Script"
echo "================================================"
echo ""

# Check if CMake is installed
if ! command -v cmake &> /dev/null
then
    echo "❌ ERROR: CMake is not installed!"
    echo ""
    echo "Please install CMake first:"
    echo "  macOS: brew install cmake"
    echo "  Linux: sudo apt install cmake"
    echo ""
    exit 1
fi

echo "✅ CMake found: $(cmake --version | head -n 1)"
echo ""

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "📁 Creating build directory..."
    mkdir build
fi

cd build

# Run CMake configuration
echo "⚙️  Configuring project with CMake..."
echo ""

# Detect platform
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS - use Xcode generator
    cmake -G Xcode ..
else
    # Linux - use default generator
    cmake ..
fi

if [ $? -ne 0 ]; then
    echo ""
    echo "❌ CMake configuration failed!"
    echo "Check the error messages above."
    exit 1
fi

echo ""
echo "✅ Configuration complete!"
echo ""

# Build the project
echo "🔨 Building project (this may take 5-10 minutes on first build)..."
echo ""

cmake --build . --config Release

if [ $? -ne 0 ]; then
    echo ""
    echo "❌ Build failed!"
    echo "Check the error messages above."
    exit 1
fi

echo ""
echo "================================================"
echo "  ✅ BUILD SUCCESSFUL!"
echo "================================================"
echo ""

# Show how to run the app
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "To run the app:"
    echo "  open Release/AudioPlayer.app"
    echo ""
    echo "Or run from this script:"
    read -p "Would you like to run the app now? (y/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        open Release/AudioPlayer.app
    fi
else
    echo "To run the app:"
    echo "  ./AudioPlayer"
    echo ""
    echo "Or run from this script:"
    read -p "Would you like to run the app now? (y/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        ./AudioPlayer
    fi
fi
