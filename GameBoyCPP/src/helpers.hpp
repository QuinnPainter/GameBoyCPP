#pragma once

std::string byteToString(byte num);
std::string ushortToString(ushort num);
ushort combineBytes(byte b1, byte b2);
byte highByte(ushort s);
byte lowByte(ushort s);
byte setBit(byte b, byte index, bool value);
bool getBit(byte b, byte index);
byte reverseByte(byte b);