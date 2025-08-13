//******************************************************************************************************
// This library is comlement of snap7 library to do mapping of buffer and define types
//
// S7 Library allow to mapping the buffer into the different S7 data types
// based on wrapper made for .Net and tested for C++. It defines other types such us PLCs, Area Sources
//
// It uses Snap7 Library 1.4, made by Davide Nardella,  http://snap7.sourceforge.net/
//
// Made by Reyan Valdes, reyanvaldes@yahoo.com
// MIT License
//******************************************************************************************************

#include "s7.h"
#include "string.h" // for memcpy


using namespace std;

static byte Mask[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

//****************************************************************************

// Get Text description of PLC Type
string S7_GetTxtPLCType (short int plcType)
{
  switch (plcType)
  {
   case S7_PLC_300_400:
         return "S7 300/400";

   case S7_PLC_1200_1500:
         return "S7 1200/1500";

   case S7_PLC_LOGO_200:
         return "LOGO/S7 200";

   case S7_PLC_SINAMICS:
         return "SINAMICS";
  }
}

//****************************************************************************

// Get data type size in bytes of the specified type
 int S7_GetDataTypeSize (int type)
{
  int size =0; // for unkown type return 0

   switch (type)
  {
   case S7_TYPE_BOOL:
   case S7_TYPE_BYTE:
   case S7_TYPE_SINT:
         size = 1;
         break;

   case S7_TYPE_WORD:
   case S7_TYPE_UINT:
   case S7_TYPE_INT:
   case S7_TYPE_DATE:
         size = 2;
         break;

   case S7_TYPE_DWORD:
   case S7_TYPE_UDINT:
   case S7_TYPE_DINT:
   case S7_TYPE_REAL:
   case S7_TYPE_TOD:
         size = 4;
         break;

   case S7_TYPE_LWORD:
   case S7_TYPE_ULINT:
   case S7_TYPE_LINT:
   case S7_TYPE_LREAL:
   case S7_TYPE_DATE_AND_TIME:
         size = 8;
         break;

   case S7_TYPE_DTL:
         size = 12;
         break;

   case S7_TYPE_STRING:
         size = 254;   // Be careful with string it is variable, depends on custom size
         break;
   };

   return size;
 }

 //****************************************************************************

 // Get the word from Transport Service Access Point (TSAP) in hex format, e.g: 10.02 => 0x1002, used by Cli_SetConnectionParams
 // Enter TSAP in format ##.## in hex
 // Return: the TSAP number in a word and decimal format
 uint16_t S7_GetWordFromTSAP ( string TSAP)
 {
    short int pos = TSAP.find ('.'); // find the delimiter into the source string

    if (pos == string::npos) {// no delimiter was found, return 0
      return 0;
    }
    else { // was found '.'
      string first  = TSAP.substr (0,pos);    // get the first argument
      string second = TSAP.substr (pos+1,2) ; // get the second argument

      // convert both TSAP arguments from hex (16) in string format to a number in decimal
      char* p; // used in the strtol function

      uint8_t number1 = strtol(first.c_str(), &p, 16); // convert string with data in any base (10, 16) to long
      uint8_t number2 = strtol(second.c_str(), &p, 16); // convert string with data in any base (10, 16) to long

      uint16_t res = number1 << 8 | number2;

      return res;
    }

 }

 //****************************************************************************
 // Get Text message of Area Source
  string S7_GetTxtAreaSource (int areaSource)
  {
      switch (areaSource)
      {
       case S7_AREA_SOURCE_I: // Inputs
             return "INPUTS";

       case S7_AREA_SOURCE_Q: // Outputs
             return "OUTPUTS";

       case S7_AREA_SOURCE_M: // Memory Marks
             return "Marks";

       case S7_AREA_SOURCE_DB: // Data Block
             return "DB";

       default:
             return "Unkown Data Source";
      };
  }


//****************************************************************************

// Get BDC and convert to byte
int S7_BDCToByte (byte B)
{
  return ((B >> 4) * 10 ) + (B & 0x0F);
}

//****************************************************************************

// Convert Byte to BDC
byte S7_ByteToBDC (int Value)
{
  return (byte) ((( Value /10 ) << 4) | (Value % 10));
}

//****************************************************************************

// Get Bit position at buffer of bytes, bits 0..7
bool S7_GetBitAt ( byte Buffer[], int Pos, int Bit)
{
  if (Bit < 0) Bit = 0;
  if (Bit > 7) Bit = 7;

  return (Buffer[Pos] & Mask[Bit]) != 0;
}


//****************************************************************************

// Set Bit position at buffer of bytes

void S7_SetBitAt ( byte Buffer[], int Pos, int Bit, bool Value)
{
  if (Bit < 0) Bit = 0;
  if (Bit > 7) Bit = 7;

  if (Value)
   Buffer[Pos] = (byte)(Buffer[Pos] | Mask[Bit]);
  else
   Buffer[Pos] = (byte)(Buffer[Pos] & ~Mask[Bit]);
}

//****************************************************************************

// Get Byte (0..255) at buffer of bytes
uint8_t S7_GetByteAt(byte Buffer[], int Pos)
{
  return Buffer[Pos]  ;
}

//****************************************************************************

// Set Byte (0..255) at buffer of bytes
void S7_SetByteAt(byte Buffer[], int Pos, uint8_t Value )
{
  Buffer [Pos] = Value;
}


//****************************************************************************

// Get SInt (-128..127) at buffer of bytes
int8_t S7_GetSIntAt(byte Buffer[], int Pos)
{
  int Value = Buffer[Pos];
  if (Value < 128)
   return Value;
  else
   return (int8_t) (Value - 256);
}

//****************************************************************************

// Set SInt (-128..127) at buffer of bytes
void S7_SetSIntAt(byte Buffer[], int Pos, int8_t Value)
{
  if (Value < -128) Value = -128;
  if (Value > 127) Value = 127;
  Buffer[Pos] = (byte)Value;
}

//****************************************************************************

// Get 16 bit unsigned value (S7 UInt) 0..65535
 uint16_t S7_GetUIntAt(byte Buffer[], int Pos)
{
   return (uint16_t)((Buffer[Pos] << 8) | Buffer[Pos + 1]);
 }

 //****************************************************************************

// Set 16 bit unsigned value (S7 UInt) 0..65535

void S7_SetUIntAt(byte Buffer[], int Pos, uint16_t Value )
{
  Buffer[Pos] = (byte)(Value >> 8);
  Buffer[Pos + 1] = (byte)(Value & 0x00FF);
}

//****************************************************************************

 // Get 16 bit unsigned value (S7 Word) 0..65535

uint16_t S7_GetWordAt(byte Buffer[], int Pos)
{
  return S7_GetUIntAt(Buffer, Pos) ;
}

//****************************************************************************
// Set 16 bit unsigned value (S7 Word) 0..65535
 void S7_SetWordAt(byte Buffer[], int Pos, uint16_t Value )
{
  S7_SetUIntAt(Buffer, Pos, Value);
 }


//****************************************************************************

// Get 16 bit signed value (S7 int) -32768..32767 at buffer of bytes
int16_t S7_GetIntAt(byte Buffer[], int Pos)
{
   return (int16_t)((Buffer[Pos] << 8) | Buffer[Pos + 1]);
}

//****************************************************************************

// Set 16 bit signed value (S7 int) -32768..32767 at buffer of bytes

void S7_SetIntAt(byte Buffer[], int Pos, int16_t Value)
{
  Buffer[Pos] = (byte)(Value >> 8);
  Buffer[Pos + 1] = (byte)(Value & 0x00FF);
}

//****************************************************************************

// Get 32 bit signed value (S7 DInt) -2147483648..2147483647
long S7_GetDIntAt(byte Buffer[], int Pos)
{
  long Result;
  Result = Buffer[Pos]; Result <<= 8;
  Result += Buffer[Pos + 1]; Result <<= 8;
  Result += Buffer[Pos + 2]; Result <<= 8;
  Result += Buffer[Pos + 3];
  return Result;
}

//****************************************************************************

// Set 32 bit signed value (S7 DInt) -2147483648..2147483647

void S7_SetDIntAt(byte Buffer[], int Pos, long Value)
{
  Buffer[Pos + 3] = (byte)(Value & 0xFF);
  Buffer[Pos + 2] = (byte)((Value >> 8) & 0xFF);
  Buffer[Pos + 1] = (byte)((Value >> 16) & 0xFF);
  Buffer[Pos] = (byte)((Value >> 24) & 0xFF);
}

//****************************************************************************

//  Get 32 bit unsigned value (S7 UDInt) 0..4294967295
uint32_t S7_GetUDIntAt(byte Buffer[], int Pos)
{
  uint32_t Result;
  Result = Buffer[Pos]; Result <<= 8;
  Result |= Buffer[Pos + 1]; Result <<= 8;
  Result |= Buffer[Pos + 2]; Result <<= 8;
  Result |= Buffer[Pos + 3];
  return Result;
}

//****************************************************************************

//  Set 32 bit unsigned value (S7 UDInt) 0..4294967295

void S7_SetUDIntAt(byte Buffer[], int Pos, uint32_t Value)
{
  Buffer[Pos + 3] = (byte)(Value & 0xFF);
  Buffer[Pos + 2] = (byte)((Value >> 8) & 0xFF);
  Buffer[Pos + 1] = (byte)((Value >> 16) & 0xFF);
  Buffer[Pos] = (byte)((Value >> 24) & 0xFF);
}


//****************************************************************************

//  Get 32 bit unsigned value (S7 UDInt) 0..4294967295
uint32_t S7_GetDWordAt(byte Buffer[], int Pos)
{
  return S7_GetUDIntAt (Buffer, Pos);
}

//****************************************************************************

//  Set 32 bit unsigned value (S7 UDInt) 0..4294967295

void S7_SetDWordAt(byte Buffer[], int Pos, uint32_t Value)
{
  S7_SetUDIntAt(Buffer, Pos, Value);
}

//****************************************************************************

// Set 64 bit unsigned value (S7 ULint) 0..18446744073709551615
uint64_t S7_GetULIntAt(byte Buffer[], int Pos)
{
  uint64_t Result;
  Result = Buffer[Pos]; Result <<= 8;
  Result |= Buffer[Pos + 1]; Result <<= 8;
  Result |= Buffer[Pos + 2]; Result <<= 8;
  Result |= Buffer[Pos + 3]; Result <<= 8;
  Result |= Buffer[Pos + 4]; Result <<= 8;
  Result |= Buffer[Pos + 5]; Result <<= 8;
  Result |= Buffer[Pos + 6]; Result <<= 8;
  Result |= Buffer[Pos + 7];
  return Result;
}

//****************************************************************************

// Set 64 bit unsigned value (S7 ULint) 0..18446744073709551615
 void S7_SetULIntAt(byte Buffer[], int Pos, uint64_t Value)
{
  Buffer[Pos + 7] = (byte)(Value & 0xFF);
  Buffer[Pos + 6] = (byte)((Value >> 8) & 0xFF);
  Buffer[Pos + 5] = (byte)((Value >> 16) & 0xFF);
  Buffer[Pos + 4] = (byte)((Value >> 24) & 0xFF);
  Buffer[Pos + 3] = (byte)((Value >> 32) & 0xFF);
  Buffer[Pos + 2] = (byte)((Value >> 40) & 0xFF);
  Buffer[Pos + 1] = (byte)((Value >> 48) & 0xFF);
  Buffer[Pos] = (byte)((Value >> 56) & 0xFF);
 }

 //****************************************************************************
 // Set 64 bit unsigned value (S7 ULint) 0..18446744073709551615

 uint64_t S7_GetLWordAt(byte Buffer[], int Pos)
 {
  return S7_GetULIntAt (Buffer, Pos)  ;
 }

 //****************************************************************************
 // Set 64 bit unsigned value (S7 ULint) 0..18446744073709551615

 void S7_SetLWordAt(byte Buffer[], int Pos, uint64_t Value)
 {
   S7_SetULIntAt ( Buffer, Pos, Value)  ;
 }


//****************************************************************************

// Get 64 bit signed value (S7 LInt) -9223372036854775808..9223372036854775807
int64_t S7_GetLIntAt(byte Buffer[], int Pos)
{
  int64_t Result;
  Result = Buffer[Pos]; Result <<= 8;
  Result += Buffer[Pos + 1]; Result <<= 8;
  Result += Buffer[Pos + 2]; Result <<= 8;
  Result += Buffer[Pos + 3]; Result <<= 8;
  Result += Buffer[Pos + 4]; Result <<= 8;
  Result += Buffer[Pos + 5]; Result <<= 8;
  Result += Buffer[Pos + 6]; Result <<= 8;
  Result += Buffer[Pos + 7];
  return Result;
}

//****************************************************************************

// Set 64 bit signed value (S7 LInt) -9223372036854775808..9223372036854775807
void S7_SetLIntAt(byte Buffer[], int Pos, int64_t Value)
{
   Buffer[Pos + 7] = (byte)(Value & 0xFF);
   Buffer[Pos + 6] = (byte)((Value >> 8) & 0xFF);
   Buffer[Pos + 5] = (byte)((Value >> 16) & 0xFF);
   Buffer[Pos + 4] = (byte)((Value >> 24) & 0xFF);
   Buffer[Pos + 3] = (byte)((Value >> 32) & 0xFF);
   Buffer[Pos + 2] = (byte)((Value >> 40) & 0xFF);
   Buffer[Pos + 1] = (byte)((Value >> 48) & 0xFF);
   Buffer[Pos] = (byte)((Value >> 56) & 0xFF);
}

//****************************************************************************
// Get 32 bit floating point number (S7 Real) (Range of float)
float S7_GetRealAt(byte Buffer[], int Pos)
{
    uint32_t Pack = S7_GetUDIntAt(Buffer, Pos);
    float  Res; memcpy (&Res, &Pack, 4);
    return Res;
}

//****************************************************************************

// Set 32 bit floating point number (S7 Real) (Range of float)
 void S7_SetRealAt(byte Buffer[], int Pos, float Value)
 {

   uint32_t Pack;
   memcpy (&Pack, &Value, 4);
   S7_SetUDIntAt (Buffer, Pos, Pack);
 }

 //****************************************************************************

 // Get 64 bit floating point number (S7 LReal) (Range of double)
 double S7_GetLRealAt(byte Buffer[], int Pos)
 {
   uint64_t Pack = S7_GetULIntAt(Buffer, Pos)  ;
   double Res; memcpy (&Res, &Pack, 8);
   return Res;
 }

 //****************************************************************************

 // Set 64 bit floating point number (S7 LReal) (Range of double)
 void S7_SetLRealAt(byte Buffer[], int Pos, double Value)
 {
     uint64_t Pack;
     memcpy (&Pack, &Value, 8);
     S7_SetULIntAt (Buffer, Pos, Pack);
 }

//****************************************************************************
// Get String (S7 String)
// In Siemens the standard string has format:
//  - 1st byte: Max Length
//  - 2nd byte: Current Length
//  - 3rd ... n byte: string characters

 string S7_GetStringAt(byte Buffer[], int Pos)
 {
   string res;

   int size = (int) Buffer[Pos + 1];

   res.insert (0, (char*) &Buffer[Pos+2],size);

   return res;
 }

 //****************************************************************************
 // Set String (S7 String)
 // In Siemens the standard string has format:
 //  - 1st byte: Max Length
 //  - 2nd byte: Current Length
 //  - 3rd ... n byte: string characters

  void S7_SetStringAt(byte Buffer[], int Pos, int MaxLen, string Value)
  {
    int size = Value.size();

    Buffer[Pos] = (byte)MaxLen;
    Buffer[Pos + 1] = (byte)size;

    Value.copy ((char*) &Buffer[Pos+2],size);
  }

 //****************************************************************************
 //Get Array of char (S7 ARRAY OF CHARS)
  string S7_GetCharsAt(byte Buffer[], int Pos, int Size)
  {
      string res;

      res.insert (0, (char*) &Buffer[Pos],Size);

      return res;
  }

#include <iostream>

 //****************************************************************************
 //Set Array of char (S7 ARRAY OF CHARS)
  void S7_SetCharsAt(byte Buffer[], int BufferLen, int Pos, string Value)
  {
   int MaxLen = BufferLen - Pos;
   int Size = Value.size();

    cout << " Max Len: " << MaxLen << " Size: " << Size << endl;

   // Truncs the string if there's no room enough
   if (Size > MaxLen) Size = MaxLen;



   Value.copy ((char*) &Buffer[Pos],Size);
  }

 //****************************************************************************
// 15-May-2023 - TOD, DTL, DATE, DATE_AND_TIME SET/GET Contribution by PiotrBzdręga 
//****************************************************************************
// Get TIME_OF_DAY hour:minute:second:millisecond (S7 TOD)
// TOD#0:0:0.0 to TOD#23:59 : 59.999
// Time in steps of 1 ms
// https://support.industry.siemens.com/cs/mdm/109773506?c=64869849355&lc=en-WW

TOD S7_GetTODAt(byte Buffer[], int Pos)
{
  uint32_t time = S7_GetUDIntAt(Buffer, Pos);
  uint32_t msec = time % 1000;
  time = (time - msec) / 1000;
  uint32_t second = time % 60;
  time = (time - second) / 60;
  uint32_t minute = time % 60;
  uint32_t hour = (time - minute) / 60;

  return TOD{hour, minute, second, msec};
}

//****************************************************************************
// Set TIME_OF_DAY hour:minute:second:millisecond (S7 TOD)
// TOD#0:0:0.0 to TOD#23:59 : 59.999
// Time in steps of 1 ms
// https://support.industry.siemens.com/cs/mdm/109773506?c=64869849355&lc=en-WW

void S7_SetTODAt(byte Buffer[], int Pos, uint32_t hour, uint32_t minute, uint32_t second, uint32_t msec)
{
  uint32_t time = msec;
  time += 1000 * second;
  time += 60 * 1000 * minute;
  time += 60 * 60 * 1000 * hour;
  S7_SetUDIntAt(Buffer, Pos, time);
}

//****************************************************************************
// Get (Year-Month-Day) (S7 DATE)
// The DATE data type saves the date as an unsigned integer. The representation contains the year, the month, and the day.
// D#1990-1-1 to D#2168 - 12 - 31
// IEC date in steps of 1 day
// https://support.industry.siemens.com/cs/mdm/109773506?c=104323664139&lc=en-PL
// http://howardhinnant.github.io/date_algorithms.html#civil_from_days

DATE S7_GetDATEAt(byte Buffer[], int Pos)
{
  const unsigned z = S7_GetUIntAt(Buffer, Pos) + 726773; //(offset) days from 0000-03-01 to 1990-01-01
  const unsigned era = z / 146097;
  const unsigned doe = (z - era * 146097);                                    // [0, 146096]
  const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
  const unsigned y = yoe + era * 400;
  const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0, 365]
  const unsigned mp = (5 * doy + 2) / 153;                      // [0, 11]
  const unsigned d = doy - (153 * mp + 2) / 5 + 1;              // [1, 31]
  const unsigned m = mp < 10 ? mp + 3 : mp - 9;                 // [1, 12]

  return DATE{y + (m <= 2), m, d};
}

//****************************************************************************
// Set (Year-Month-Day) (S7 DATE)
// The DATE data type saves the date as an unsigned integer. The representation contains the year, the month, and the day.
// D#1990-1-1 to D#2168 - 12 - 31
// IEC date in steps of 1 day
// https://support.industry.siemens.com/cs/mdm/109773506?c=104323664139&lc=en-PL
// http://howardhinnant.github.io/date_algorithms.html#days_from_civil

void S7_SetDATEAt(byte Buffer[], int Pos, uint32_t year, uint32_t month, uint32_t day)
{
  year -= month <= 2;
  const unsigned era = year / 400;
  const unsigned yoe = static_cast<unsigned>(year - era * 400);                       // [0, 399]
  const unsigned doy = (153 * (month > 2 ? month - 3 : month + 9) + 2) / 5 + day - 1; // [0, 365]
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;                         // [0, 146096]
  uint32_t Value = era * 146097 + static_cast<unsigned>(doe) - 726773;                //[726773] (offset) days from 0000-03-01 to 1990-01-01
  S7_SetUIntAt(Buffer, Pos, Value);
}

//****************************************************************************
// Get year - month - day - hour:minute:second:millisecond (S7 DATE_AND_TIME)
// The DT (DATE_AND_TIME) data type saves the information on date and time of day in BCD format.
// Min.: DT#1990-01-01-00:00:00.000 Max.: DT#2089 - 12 - 31 - 23:59 : 59.999
// https://support.industry.siemens.com/cs/mdm/109773506?c=85672757259&lc=en-PL

DATE_AND_TIME S7_GetDATE_AND_TIMEAt(byte Buffer[], int Pos)
{
  uint16_t year = S7_BDCToByte(Buffer[Pos]);                                            // [0, 99]
  year += (year >= 90 ? 1900 : 2000);                                                   //(Years // [1990, 2089]); BCD#90 = 1990; (...) BCD#0 = 2000; (...) BCD#89 = 2089
  uint16_t month = S7_BDCToByte(Buffer[Pos + 1]);                                       // [1, 12]
  uint16_t day = S7_BDCToByte(Buffer[Pos + 2]);                                         // [1, 31]
  uint16_t hour = S7_BDCToByte(Buffer[Pos + 3]);                                        // [0, 23]
  uint16_t minute = S7_BDCToByte(Buffer[Pos + 4]);                                      // [0, 59]
  uint16_t second = S7_BDCToByte(Buffer[Pos + 5]);                                      // [0, 59]
  uint16_t msec = S7_BDCToByte(Buffer[Pos + 6]) * 10 + ((Buffer[Pos + 7] >> 4) & 0x0F); // [0, 999] 7 (4MSB)
  uint16_t weekday = (Buffer[Pos + 7] & 0x0F);                                          //[1, 7]; #1 = Sunday; #7 = Saturday 7 (4LSB)

  return DATE_AND_TIME{year, month, day, hour, minute, second, msec, weekday};
}

//****************************************************************************
// Set year - month - day - hour:minute:second:millisecond (S7 DATE_AND_TIME)
// The DT (DATE_AND_TIME) data type saves the information on date and time of day in BCD format.
// Min.: DT#1990-01-01-00:00:00.000 Max.: DT#2089 - 12 - 31 - 23:59 : 59.999
// https://support.industry.siemens.com/cs/mdm/109773506?c=85672757259&lc=en-PL
void S7_SetDATE_AND_TIMEAt(byte Buffer[], int Pos, uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t minute, uint16_t second, uint16_t msec)
{
  //(Years // [1990, 2089]); BCD#90 = 1990; (...) BCD#0 = 2000; (...) BCD#89 = 2089
  Buffer[Pos] = S7_ByteToBDC(year % 100);    // [0, 89]
  Buffer[Pos + 1] = S7_ByteToBDC(month);     // [1, 12]
  Buffer[Pos + 2] = S7_ByteToBDC(day);       // [1, 31]
  Buffer[Pos + 3] = S7_ByteToBDC(hour);      // [0, 23]
  Buffer[Pos + 4] = S7_ByteToBDC(minute);    // [0, 59]
  Buffer[Pos + 5] = S7_ByteToBDC(second);    // [0, 59]
  Buffer[Pos + 6] = S7_ByteToBDC(msec / 10); // [0, 99]

  // weekday calculation
  year -= month <= 2;
  const unsigned era = year / 400;
  const unsigned yoe = static_cast<unsigned>(year - era * 400);                       // [0, 399]
  const unsigned doy = (153 * (month > 2 ? month - 3 : month + 9) + 2) / 5 + day - 1; // [0, 365]
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;                         // [0, 146096]
  uint32_t days = era * 146097 + static_cast<unsigned>(doe) - 726773;
  uint8_t weekday = ((days + 1) % 7) + 1; // 1990-01-01[Monday(1)] [1,7]

  Buffer[Pos + 7] = weekday;             // [1, 7]
  Buffer[Pos + 7] |= ((msec % 10) << 4); // [0, 9]
}


//****************************************************************************
// Get Year-Month-Day-Hour:Minute:Second.Nanoseconds (S7 DTL)
// An operand of data type DTL has a length of 12 bytes and stores date and time information in a predefined structure.
// Min.: DTL#1970-01-01-00:00:00.0  Max.: DTL#2262 - 04 - 11 - 23:47 : 16.854775807
// https://support.industry.siemens.com/cs/mdm/109773506?c=93833257483&lc=en-PL
DTL S7_GetDTLAt(byte Buffer[], int Pos)
{
  uint16_t year = S7_GetUIntAt(Buffer, Pos);                               // 1970, 2262]
  uint16_t month = static_cast<uint16_t>(S7_GetByteAt(Buffer, Pos + 2));   // [1, 12]
  uint16_t day = static_cast<uint16_t>(S7_GetByteAt(Buffer, Pos + 3));     // [1, 31]
  uint16_t weekday = static_cast<uint16_t>(S7_GetByteAt(Buffer, Pos + 4)); // [1, 7]
  uint16_t hour = static_cast<uint16_t>(S7_GetByteAt(Buffer, Pos + 5));    // [0, 23]
  uint16_t minute = static_cast<uint16_t>(S7_GetByteAt(Buffer, Pos + 6));  // [0, 59]
  uint16_t second = static_cast<uint16_t>(S7_GetByteAt(Buffer, Pos + 7));  // [0, 59]
  uint32_t nanosec = S7_GetUDIntAt(Buffer, Pos + 8);                       // [0, 999999999]

  return DTL{year, month, day, weekday, hour, minute, second, nanosec};
}

//****************************************************************************
// Set Year-Month-Day-Hour:Minute:Second.Nanoseconds (S7 DTL)
// An operand of data type DTL has a length of 12 bytes and stores date and time information in a predefined structure.
// Min.: DTL#1970-01-01-00:00:00.0  Max.: DTL#2262 - 04 - 11 - 23:47 : 16.854775807
// https://support.industry.siemens.com/cs/mdm/109773506?c=93833257483&lc=en-PL

void S7_SetDTLAt(byte Buffer[], int Pos, uint16_t year, uint16_t month, uint16_t day, uint16_t hour, uint16_t minute, uint16_t second, uint32_t nanosec)
{
  S7_SetUIntAt(Buffer, Pos, year);      // 1970, 2262]
  S7_SetByteAt(Buffer, Pos + 2, month); // [1, 12]
  S7_SetByteAt(Buffer, Pos + 3, day);   // [1, 31]

  // weekday calculation
  year -= month <= 2;
  const unsigned era = year / 400;
  const unsigned yoe = static_cast<unsigned>(year - era * 400);                       // [0, 399]
  const unsigned doy = (153 * (month > 2 ? month - 3 : month + 9) + 2) / 5 + day - 1; // [0, 365]
  const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;                         // [0, 146096]
  uint32_t days = era * 146097 + static_cast<unsigned>(doe) - 726773;
  uint8_t weekday = ((days + 1) % 7) + 1; // 1990-01-01[Monday(1)] [1,7]

  S7_SetByteAt(Buffer, Pos + 4, weekday);  // [1, 7]
  S7_SetByteAt(Buffer, Pos + 5, hour);     // [0, 23]
  S7_SetByteAt(Buffer, Pos + 6, minute);   // [0, 59]
  S7_SetByteAt(Buffer, Pos + 7, second);   // [0, 59]
  S7_SetUDIntAt(Buffer, Pos + 8, nanosec); // [0, 999999999]
}
