#pragma once

class CUnicodeTextFile
{
public:
  CUnicodeTextFile();
  ~CUnicodeTextFile();

  enum endian 
  {
    little_endian, // least significant byte first or reverse byte order - Intel x86, ...
    big_endian     // most significant byte first - Motorola, Sparc, MIPS, ...
  };
  static endian Endian(int); // convert integer to endian enum
  static endian Endian();    // returns endian-ness of current CPU

  enum filetype
  {
    unknown_filetype,
    ansi_filetype,
    utf8_filetype,
    utf16_filetype,
  };
  static filetype FileType(int); // convert integer to filetype enum

  enum status
  {
    no_status,
    read_status,
    write_status,
  };

  bool OpenToRead( const wchar_t* pszFileName );
  bool OpenToWrite( const wchar_t* pszFileName, filetype ft = ansi_filetype );
  bool OpenToAppend( const wchar_t* pszFileName );

  filetype GetFileType() const;
  bool ReadString( CString& string );
  bool WriteString( const wchar_t* pszString );

  void Close();

private:
  FILE* m_fp;
  endian m_cpu_endian;
  endian m_file_endian;
  status m_status;
  filetype m_filetype;
  bool m_bWriteBOM;

private:
  CUnicodeTextFile( const CUnicodeTextFile& );
  CUnicodeTextFile& operator=(const CUnicodeTextFile&);

  bool ValidateToRead();
  bool RewindFilePointer( fpos_t pos_distance );
  bool ToggleByteOrder( int count, int sizeof_element, const void* src, void* dst );
  bool WriteBOM();
};
