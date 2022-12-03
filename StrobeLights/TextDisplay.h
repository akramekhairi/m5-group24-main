#pragma once
#include <string>
// Get character data from the APT arrays
void CharacterExtractor(char a, unsigned char* output5x5);

// Bridge between two characters, output via output5x5 (array of 25 positions)
void BridgeCharacters(unsigned char* a5x5, unsigned char* b5x5, unsigned char* output5x5, unsigned char step, unsigned char type);

// Add color to 5x5 3-color array
void Colorize5x5(unsigned char* output5x5, unsigned char R, unsigned char G, unsigned char B);

// Check if character is lowercase
bool isLowercase(char a);

// Add x-y padding
void AddPadding(unsigned char* output5x5, unsigned char x, unsigned char y);
void RemovePadding(unsigned char* output5x5);

// Function responsible for creating the scrolling text effect
void TextDisplay(const std::string& s, float characterDuration, unsigned char R = 0x57, unsigned char G = 0x06, unsigned char B = 0x8c);

// Cubic interpolation between a and b by t in range [0, 1]
float cubicInterpolation(float a, float b, float t);

// Function to display a single character
void CharacterDisplay(char a, float characterDuration, unsigned char R = 0x57, unsigned char G = 0x06, unsigned char B = 0x8c,
  bool fadeIn = false, bool fadeOut = false, float fadeDuration = 0, bool keep = false);
