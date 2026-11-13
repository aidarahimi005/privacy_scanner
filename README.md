# Privacy Scanner

A character-based lexical scanner written in C for detecting and sanitizing sensitive information (Email, Iranian Phone, IPv4) from UTF-8 text files. 

This project was developed as the practical assignment for the Compiler Design course at Amirkabir University of Technology (Tehran Polytechnic).

## Features

- **Strict Character-Based Scanning**: Implements a custom scanner without relying on Regex libraries or external pattern-matching tools.
- **Candidate Extraction**: Uses the Longest Match rule to extract continuous sequences of valid characters.
- **Complete Pattern Matching**: Ensures the *entire* candidate matches the pattern (rejects partial matches like `abc09123456789`).
- **Email Validation**: Validates local-part (alphanumeric, `.`, `-`, `_`) and domain (labels, TLD with at least 2 letters).
- **Iranian Phone Validation**: Validates exactly 11 digits starting with `09` (no spaces or dashes).
- **IPv4 Validation**: Validates 4 octets (0-255, no leading zeros).
- **UTF-8 & CRLF Support**: Preserves Persian text, punctuation, and Windows line endings (`\r\n`). Columns are counted based on Unicode characters, not bytes.
- **Sanitization**: Replaces sensitive data with `[EMAIL]`, `[PHONE]`, and `[IP]` tags.
- **JSON Reporting**: Generates a structured `report.json` with the exact line, column, and length for each finding.

## Project Structure

```text
privacy_scanner/
├── include/
│   ├── patterns.h
│   ├── report.h
│   └── scanner.h
├── src/
│   ├── main.c
│   ├── patterns.c
│   ├── report.c
│   └── scanner.c
├── Makefile
└── README.md
