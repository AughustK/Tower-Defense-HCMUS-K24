#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Helper for loading animations from a sprite sheet
namespace AnimationLoader {
    // Load a horizontal strip animation (all frames in one row)
    inline std::vector<AnimationFrame> LoadFromHorizontalStrip(
        int frameWidth,
        int frameHeight,
        int frameCount,
        int yPosition = 0,
        float frameDuration = 0.1f)
    {
        std::vector<AnimationFrame> frames;
        for (int i = 0; i < frameCount; i++) {
            sf::IntRect rect(i * frameWidth, yPosition, frameWidth, frameHeight);
            frames.emplace_back(rect, frameDuration);
        }
        return frames;
    }

    // Load a grid-based animation (frames arranged in rows and columns)
    inline std::vector<AnimationFrame> LoadFromGrid(
        int frameWidth,
        int frameHeight,
        int columns,
        int rows,
        int startFrame = 0,
        int endFrame = -1,
        float frameDuration = 0.1f)
    {
        std::vector<AnimationFrame> frames;
        int totalFrames = columns * rows;
        if (endFrame < 0 || endFrame >= totalFrames)
            endFrame = totalFrames - 1;
        for (int i = startFrame; i <= endFrame; i++) {
            int col = i % columns;
            int row = i / columns;
            sf::IntRect rect(col * frameWidth, row * frameHeight, frameWidth, frameHeight);
            frames.emplace_back(rect, frameDuration);
        }
        return frames;
    }
}
