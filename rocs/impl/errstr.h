/*
 Rocs - OS independent C library

 Copyright (C) 2002-2014 Rob Versluis, Rocrail.net

 


 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef __ROCS_ERRSTR_IMPL_H
#define __ROCS_ERRSTR_IMPL_H

#if defined __APPLE__ || defined __OpenBSD__
static const char* errStr[] = {
  "OK",
  "Operation not permitted",
  "No such file or directory",
  "No such process",
  "Interrupted system call",
  "Input/output error",
  "Device not configured",
  "Argument list too long",
  "Exec format error",
  "Bad file descriptor",
  "No child processes",
  "Resource deadlock avoided",
  "Cannot allocate memory",
  "Permission denied",
  "Bad address",
  "Block device required",
  "Device / Resource busy",
  "File exists",
  "Cross-device link",
  "Operation not supported by device",
  "Not a directory",
  "Is a directory",
  "Invalid argument",
  "Too many open files in system",
  "Too many open files",
  "Inappropriate ioctl for device",
  "Text file busy",
  "File too large",
  "No space left on device",
  "Illegal seek",
  "Read-only file system",
  "Too many links",
  "Broken pipe",
  "Numerical argument out of domain",
  "Result too large",
  "Resource temporarily unavailable",
  "Operation now in progress",
  "Operation already in progress",
  "Socket operation on non-socket",
  "Destination address required",
  "Message too long",
  "Protocol wrong type for socket",
  "Protocol not available",
  "Protocol not supported",
  "Socket type not supported",
  "Operation not supported",
  "Protocol family not supported",
  "Address family not supported by protocol family",
  "Address already in use",
  "Can't assign requested address",
  "Network is down",
  "Network is unreachable",
  "Network dropped connection on reset",
  "Software caused connection abort",
  "Connection reset by peer",
  "No buffer space available",
  "Socket is already connected",
  "Socket is not connected",
  "Can't send after socket shutdown",
  "Too many references: can't splice",
  "Operation timed out",
  "Connection refused",
  "Too many levels of symbolic links",
  "File name too long",
  "Host is down",
  "No route to host",
  "Directory not empty",
  "Too many processes",
  "Too many users",
  "Disc quota exceeded",
  "Stale NFS file handle",
  "Too many levels of remote in path",
  "RPC struct is bad",
  "RPC version wrong",
  "RPC prog. not avail",
  "Program version wrong",
  "Bad procedure for program",
  "No locks available",
  "Function not implemented",
  "Inappropriate file type or format",
  "Authentication error",
  "Need authenticator",
  "Device power is off",
  "Device error, e.g. paper out",
  "Value too large to be stored in data type",
  "Bad executable",
  "Bad CPU type in executable",
  "Shared library version mismatch",
  "Malformed Macho file",
  "Operation canceled",
  "Identifier removed",
  "No message of desired type",
  "Illegal byte sequence",
  "Attribute not found",
  "Bad message",
  "Reserved",
  "No message available on STREAM",
  "Reserved",
  "No STREAM resources",
  "Not a STREAM",
  "Protocol error",
  "STREAM ioctl timeout",
  "Operation not supported on socket",
  "No such policy registered",
  "State not recoverable",
  "Previous owner died",
  "Interface output queue is full"
};
#define ERRSTR_LAST 106
#else
static const char* errStr[] = {
  "OK",
  "Operation not permitted",
  "No such file or directory",
  "No such process",
  "Interrupted system call",
  "I/O error",
  "No such device or address",
  "Arg list too long",
  "Exec format error",
  "Bad file number",
  "No child processes",
  "Try again",
  "Out of memory",
  "Permission denied", 
  "Bad address",
  "Block device required",
  "Device or resource busy",
  "File exists",
  "Cross-device link",
  "No such device",
  "Not a directory",
  "Is a directory",
  "Invalid argument",
  "File table overflow",
  "Too many open files",
  "Not a typewriter",
  "Text file busy",
  "File too large",
  "No space left on device",
  "Illegal seek",
  "Read-only file system",
  "Too many links",
  "Broken pipe",
  "Math argument out of domain of func",
  "Math result not representable",
  "Resource deadlock would occur",
  "File name too long",
  "No record locks available",
  "Function not implemented",
  "Directory not empty",
  "Too many symbolic links encountered",
  "Operation would block",
  "No message of desired type",
  "Identifier removed",
  "Channel number out of range",
  "Level 2 not synchronized",
  "Level 3 halted",
  "Level 3 reset",
  "Link number out of range",
  "Protocol driver not attached",
  "No CSI structure available",
  "Level 2 halted",
  "Invalid exchange",
  "Invalid request descriptor",
  "Exchange full",
  "No anode",
  "Invalid request code",
  "Invalid slot",
  "Resource deadlock would occur",
  "Bad font file format",
  "Device not a stream",
  "No data available",
  "Timer expired",
  "Out of streams resources",
  "Machine is not on the network",
  "Package not installed",
  "Object is remote",
  "Link has been severed",
  "Advertise error",
  "Srmount error",
  "Communication error on send",
  "Protocol error",
  "Multihop attempted",
  "RFS specific error",
  "Not a data message",
  "Value too large for defined data type",
  "Name not unique on network",
  "File descriptor in bad state",
  "Remote address changed",
  "Can not access a needed shared library",
  "Accessing a corrupted shared library",
  ".lib section in a.out corrupted",
  "Attempting to link in too many shared libraries",
  "Cannot exec a shared library directly",
  "Illegal byte sequence",
  "Interrupted system call should be restarted",
  "Streams pipe error",
  "Too many users",
  "Socket operation on non-socket",
  "Destination address required",
  "Message too long",
  "Protocol wrong type for socket",
  "Protocol not available",
  "Protocol not supported",
  "Socket type not supported",
  "Operation not supported on transport endpoint",
  "Protocol family not supported",
  "Address family not supported by protocol",
  "Address already in use",
  "Cannot assign requested address",
  "Network is down",
  "Network is unreachable",
  "Network dropped connection because of reset",
  "Software caused connection abort",
  "Connection reset by peer",
  "No buffer space available",
  "Transport endpoint is already connected",
  "Transport endpoint is not connected",
  "Cannot send after transport endpoint shutdown",
  "Too many references: cannot splice",
  "Connection timed out",
  "Connection refused",
  "Host is down",
  "No route to host",
  "Operation already in progress",
  "Operation now in progress",
  "Stale NFS file handle",
  "Structure needs cleaning",
  "Not a XENIX named type file",
  "No XENIX semaphores available",
  "Is a named type file",
  "Remote I/O error",
  "Quota exceeded",
  "No medium found",
  "Wrong medium type"
};
#define ERRSTR_LAST 124
#endif

static const char* sigStr[] = {
  "",
  "Hangup",
  "Interrupt",
  "Quit",
  "Illegal instruction",
  "Trace trap",
  "Abort",
  "BUS error",
  "Floating-point exception",
  "Kill, unblockable",
  "User-defined signal 1",
  "Segmentation violation",
  "User-defined signal 2",
  "Broken pipe",
  "Alarm clock",
  "Termination",
  "Stack fault",
  "Child status has changed",
  "Continue",
  "Stop, unblockable",
  "Keyboard stop",
  "Background read from tty",
  "Background write to tty",
  "Urgent condition on socket",
  "CPU limit exceeded",
  "File size limit exceeded",
  "Virtual alarm clock",
  "Profiling alarm clock",
  "",
  "I/O now possible",
  "Power failure restart",
  "Bad system call",
};  
#define SIGSTR_LAST 31

#endif
