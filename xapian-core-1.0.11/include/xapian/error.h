/** @file error.h
 *  @brief Hierarchy of classes which Xapian can throw as exceptions.
 */
/* Warning: This file is generated by ./generate-exceptions - do not modify directly! */
/* Copyright (C) 2003,2004,2006,2007 Olly Betts
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef XAPIAN_INCLUDED_ERROR_H
#define XAPIAN_INCLUDED_ERROR_H

#include <string>
#include <xapian/deprecated.h>
#include <xapian/visibility.h>

namespace Xapian {

class ErrorHandler;

/** All exceptions thrown by Xapian are subclasses of Xapian::Error.
 *
 *  This class can not be instantiated directly - instead a subclass should
 *  be used.
 */
class XAPIAN_VISIBILITY_DEFAULT Error {
    // ErrorHandler needs to be able to access Error::already_handled.
    friend class ErrorHandler;

    /// Message giving details of the error, intended for human consumption.
    std::string msg;

    /** Optional context information.
     *
     *  This context is intended for use by Xapian::ErrorHandler (for example
     *  so it can know which remote server is unreliable and report the problem
     *  and remove that server from those being searched).  But it's typically
     *  a plain-text string, and so also fit for human consumption.
     */
    std::string context;

    /// The type of this error (e.g. DocNotFoundError.)
    const char * type;

    /** Optional value of 'errno' associated with this error.
     *
     *  If no value is associated, this member variable will be 0.
     *
     *  On UNIX, if this value is < 0, it's a negated h_errno value (giving
     *  an error from gethostbyname() or similar).
     *
     *  On Windows, if this value is < 0, it's a negated Windows error code
     *  (as given by GetLastError() or WSAGetLastError()).
     *
     *  NB We don't just call this member "errno" to avoid problems on
     *  platforms where errno is a preprocessor macro.
     */
    int my_errno;

    /** The error string derived from my_errno.
     *
     *  This string is generated from my_errno lazily.
     */
    mutable std::string error_string;

    /// True if this error has already been passed to an ErrorHandler.
    bool already_handled;

    /// Don't allow assignment of the base class.
    void operator=(const Error &o);

  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    Error(const std::string &msg_, const std::string &context_,
	  const char * type_, const char * error_string_);

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    Error(const std::string &msg_, const std::string &context_,
	  const char * type_, int errno_)
	: msg(msg_), context(context_), type(type_), my_errno(errno_),
	  error_string(), already_handled(false) { }

  public:
    /// The type of this error (e.g. "DocNotFoundError".)
    const char * get_type() const { return type; }

    /// Message giving details of the error, intended for human consumption.
    const std::string & get_msg() const { return msg; }

    /** Optional context information.
     *
     *  This context is intended for use by Xapian::ErrorHandler (for example
     *  so it can know which remote server is unreliable and report the problem
     *  and remove that server from those being searched).  But it's typically
     *  a plain-text string, and so also fit for human consumption.
     */
    const std::string & get_context() const { return context; }

    /** Returns any system error string associated with this exception.
     *
     *  The system error string may come from errno, h_errno (on UNIX), or
     *  GetLastError() (on MS Windows).  If there is no associated system
     *  error string, NULL is returned.
     */
    const char * get_error_string() const;

    /** Optional value of 'errno' associated with this error.
     *
     *  If no 'errno' value is associated, returns 0.  If the returned value
     *  is negative, it's a platform-specific error code (on UNIX, -h_errno;
     *  on MS Windows, -GetLastError()).
     *
     *  @deprecated This method is deprecated, because errno values aren't
     *  portable between platforms, so we can't serialise them when passing
     *  exceptions from a remote server to a client.  Use the
     *  get_error_string() method instead.
     */
    XAPIAN_DEPRECATED(int get_errno() const);

    /// Return a string describing this object.
    std::string get_description() const;
};

inline int Xapian::Error::get_errno() const { return my_errno; }

/** The base class for exceptions indicating errors in the program logic.
 *
 *  A subclass of LogicError will be thrown if Xapian detects a violation
 *  of a class invariant or a logical precondition or postcondition, etc.
 */
class XAPIAN_VISIBILITY_DEFAULT LogicError : public Error {
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    LogicError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: Error(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    LogicError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: Error(msg_, context_, type_, errno_) {}
};

/** The base class for exceptions indicating errors only detectable at runtime.
 *
 *  A subclass of RuntimeError will be thrown if Xapian detects an error
 *  which is exception derived from RuntimeError is thrown when an
 *  error is caused by problems with the data or environment rather
 *  than a programming mistake.
 */
class XAPIAN_VISIBILITY_DEFAULT RuntimeError : public Error {
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    RuntimeError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: Error(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    RuntimeError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: Error(msg_, context_, type_, errno_) {}
};

/** AssertionError is thrown if a logical assertion inside Xapian fails.
 *
 *  In a debug build of Xapian, a failed assertion in the core library code
 *  will cause AssertionError to be thrown.
 *
 *  This represents a bug in Xapian (either an invariant, precondition, etc
 *  has been violated, or the assertion is incorrect!)
 */
class XAPIAN_VISIBILITY_DEFAULT AssertionError : public LogicError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    AssertionError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: LogicError(msg_, context_, "AssertionError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit AssertionError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: LogicError(msg_, context_, "AssertionError", errno_) {}
    /** Construct from message and errno value. */
    AssertionError(const std::string &msg_, int errno_)
	: LogicError(msg_, "", "AssertionError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    AssertionError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: LogicError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    AssertionError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: LogicError(msg_, context_, type_, errno_) {}
};

/** InvalidArgumentError indicates an invalid parameter value was passed to the API.
*/
class XAPIAN_VISIBILITY_DEFAULT InvalidArgumentError : public LogicError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    InvalidArgumentError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: LogicError(msg_, context_, "InvalidArgumentError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit InvalidArgumentError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: LogicError(msg_, context_, "InvalidArgumentError", errno_) {}
    /** Construct from message and errno value. */
    InvalidArgumentError(const std::string &msg_, int errno_)
	: LogicError(msg_, "", "InvalidArgumentError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    InvalidArgumentError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: LogicError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    InvalidArgumentError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: LogicError(msg_, context_, type_, errno_) {}
};

/** InvalidOperationError indicates the API was used in an invalid way.
 */
class XAPIAN_VISIBILITY_DEFAULT InvalidOperationError : public LogicError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    InvalidOperationError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: LogicError(msg_, context_, "InvalidOperationError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit InvalidOperationError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: LogicError(msg_, context_, "InvalidOperationError", errno_) {}
    /** Construct from message and errno value. */
    InvalidOperationError(const std::string &msg_, int errno_)
	: LogicError(msg_, "", "InvalidOperationError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    InvalidOperationError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: LogicError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    InvalidOperationError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: LogicError(msg_, context_, type_, errno_) {}
};

/** UnimplementedError indicates an attempt to use an unimplemented feature. */
class XAPIAN_VISIBILITY_DEFAULT UnimplementedError : public LogicError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    UnimplementedError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: LogicError(msg_, context_, "UnimplementedError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit UnimplementedError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: LogicError(msg_, context_, "UnimplementedError", errno_) {}
    /** Construct from message and errno value. */
    UnimplementedError(const std::string &msg_, int errno_)
	: LogicError(msg_, "", "UnimplementedError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    UnimplementedError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: LogicError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    UnimplementedError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: LogicError(msg_, context_, type_, errno_) {}
};

/** DatabaseError indicates some sort of database related error. */
class XAPIAN_VISIBILITY_DEFAULT DatabaseError : public RuntimeError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    DatabaseError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: RuntimeError(msg_, context_, "DatabaseError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit DatabaseError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: RuntimeError(msg_, context_, "DatabaseError", errno_) {}
    /** Construct from message and errno value. */
    DatabaseError(const std::string &msg_, int errno_)
	: RuntimeError(msg_, "", "DatabaseError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: RuntimeError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: RuntimeError(msg_, context_, type_, errno_) {}
};

/** DatabaseCorruptError indicates database corruption was detected. */
class XAPIAN_VISIBILITY_DEFAULT DatabaseCorruptError : public DatabaseError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    DatabaseCorruptError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: DatabaseError(msg_, context_, "DatabaseCorruptError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit DatabaseCorruptError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: DatabaseError(msg_, context_, "DatabaseCorruptError", errno_) {}
    /** Construct from message and errno value. */
    DatabaseCorruptError(const std::string &msg_, int errno_)
	: DatabaseError(msg_, "", "DatabaseCorruptError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseCorruptError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: DatabaseError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseCorruptError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: DatabaseError(msg_, context_, type_, errno_) {}
};

/** DatabaseCreateError indicates a failure to create a database. */
class XAPIAN_VISIBILITY_DEFAULT DatabaseCreateError : public DatabaseError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    DatabaseCreateError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: DatabaseError(msg_, context_, "DatabaseCreateError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit DatabaseCreateError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: DatabaseError(msg_, context_, "DatabaseCreateError", errno_) {}
    /** Construct from message and errno value. */
    DatabaseCreateError(const std::string &msg_, int errno_)
	: DatabaseError(msg_, "", "DatabaseCreateError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseCreateError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: DatabaseError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseCreateError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: DatabaseError(msg_, context_, type_, errno_) {}
};

/** DatabaseLockError indicates failure to lock a database. */
class XAPIAN_VISIBILITY_DEFAULT DatabaseLockError : public DatabaseError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    DatabaseLockError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: DatabaseError(msg_, context_, "DatabaseLockError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit DatabaseLockError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: DatabaseError(msg_, context_, "DatabaseLockError", errno_) {}
    /** Construct from message and errno value. */
    DatabaseLockError(const std::string &msg_, int errno_)
	: DatabaseError(msg_, "", "DatabaseLockError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseLockError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: DatabaseError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseLockError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: DatabaseError(msg_, context_, type_, errno_) {}
};

/** DatabaseModifiedError indicates a database was modified.
 *
 *  To recover after catching this error, you need to call
 *  Xapian::Database::reopen() on the Database and repeat the operation
 *  which failed.
 */
class XAPIAN_VISIBILITY_DEFAULT DatabaseModifiedError : public DatabaseError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    DatabaseModifiedError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: DatabaseError(msg_, context_, "DatabaseModifiedError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit DatabaseModifiedError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: DatabaseError(msg_, context_, "DatabaseModifiedError", errno_) {}
    /** Construct from message and errno value. */
    DatabaseModifiedError(const std::string &msg_, int errno_)
	: DatabaseError(msg_, "", "DatabaseModifiedError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseModifiedError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: DatabaseError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseModifiedError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: DatabaseError(msg_, context_, type_, errno_) {}
};

/** DatabaseOpeningError indicates failure to open a database. */
class XAPIAN_VISIBILITY_DEFAULT DatabaseOpeningError : public DatabaseError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    DatabaseOpeningError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: DatabaseError(msg_, context_, "DatabaseOpeningError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit DatabaseOpeningError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: DatabaseError(msg_, context_, "DatabaseOpeningError", errno_) {}
    /** Construct from message and errno value. */
    DatabaseOpeningError(const std::string &msg_, int errno_)
	: DatabaseError(msg_, "", "DatabaseOpeningError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseOpeningError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: DatabaseError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseOpeningError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: DatabaseError(msg_, context_, type_, errno_) {}
};

/** DatabaseVersionError indicates that a database is in an unsupported format.
 *
 *  From time to time, new versions of Xapian will require the database format
 *  to be changed, to allow new information to be stored or new optimisations
 *  to be performed.  Backwards compatibility will sometimes be maintained, so
 *  that new versions of Xapian can open old databases, but in some cases
 *  Xapian will be unable to open a database because it is in too old (or new)
 *  a format.  This can be resolved either be upgrading or downgrading the
 *  version of Xapian in use, or by rebuilding the database from scratch with
 *  the current version of Xapian.
 */
class XAPIAN_VISIBILITY_DEFAULT DatabaseVersionError : public DatabaseOpeningError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    DatabaseVersionError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: DatabaseOpeningError(msg_, context_, "DatabaseVersionError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit DatabaseVersionError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: DatabaseOpeningError(msg_, context_, "DatabaseVersionError", errno_) {}
    /** Construct from message and errno value. */
    DatabaseVersionError(const std::string &msg_, int errno_)
	: DatabaseOpeningError(msg_, "", "DatabaseVersionError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseVersionError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: DatabaseOpeningError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DatabaseVersionError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: DatabaseOpeningError(msg_, context_, type_, errno_) {}
};

/** Indicates an attempt to access a document not present in the database. */
class XAPIAN_VISIBILITY_DEFAULT DocNotFoundError : public RuntimeError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    DocNotFoundError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: RuntimeError(msg_, context_, "DocNotFoundError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit DocNotFoundError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: RuntimeError(msg_, context_, "DocNotFoundError", errno_) {}
    /** Construct from message and errno value. */
    DocNotFoundError(const std::string &msg_, int errno_)
	: RuntimeError(msg_, "", "DocNotFoundError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DocNotFoundError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: RuntimeError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    DocNotFoundError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: RuntimeError(msg_, context_, type_, errno_) {}
};

/** Indicates an attempt to use a feature which is unavailable.
 *
 *  Typically a feature is unavailable because it wasn't compiled in, or
 *  because it requires other software or facilities which aren't available.
 */
class XAPIAN_VISIBILITY_DEFAULT FeatureUnavailableError : public RuntimeError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    FeatureUnavailableError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: RuntimeError(msg_, context_, "FeatureUnavailableError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit FeatureUnavailableError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: RuntimeError(msg_, context_, "FeatureUnavailableError", errno_) {}
    /** Construct from message and errno value. */
    FeatureUnavailableError(const std::string &msg_, int errno_)
	: RuntimeError(msg_, "", "FeatureUnavailableError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    FeatureUnavailableError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: RuntimeError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    FeatureUnavailableError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: RuntimeError(msg_, context_, type_, errno_) {}
};

/** InternalError indicates a runtime problem of some sort. */
class XAPIAN_VISIBILITY_DEFAULT InternalError : public RuntimeError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    InternalError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: RuntimeError(msg_, context_, "InternalError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit InternalError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: RuntimeError(msg_, context_, "InternalError", errno_) {}
    /** Construct from message and errno value. */
    InternalError(const std::string &msg_, int errno_)
	: RuntimeError(msg_, "", "InternalError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    InternalError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: RuntimeError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    InternalError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: RuntimeError(msg_, context_, type_, errno_) {}
};

/** Indicates a problem communicating with a remote database. */
class XAPIAN_VISIBILITY_DEFAULT NetworkError : public RuntimeError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    NetworkError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: RuntimeError(msg_, context_, "NetworkError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit NetworkError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: RuntimeError(msg_, context_, "NetworkError", errno_) {}
    /** Construct from message and errno value. */
    NetworkError(const std::string &msg_, int errno_)
	: RuntimeError(msg_, "", "NetworkError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    NetworkError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: RuntimeError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    NetworkError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: RuntimeError(msg_, context_, type_, errno_) {}
};

/** Indicates a timeout expired while communicating with a remote database. */
class XAPIAN_VISIBILITY_DEFAULT NetworkTimeoutError : public NetworkError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    NetworkTimeoutError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: NetworkError(msg_, context_, "NetworkTimeoutError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit NetworkTimeoutError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: NetworkError(msg_, context_, "NetworkTimeoutError", errno_) {}
    /** Construct from message and errno value. */
    NetworkTimeoutError(const std::string &msg_, int errno_)
	: NetworkError(msg_, "", "NetworkTimeoutError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    NetworkTimeoutError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: NetworkError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    NetworkTimeoutError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: NetworkError(msg_, context_, type_, errno_) {}
};

/** Indicates a query string can't be parsed. */
class XAPIAN_VISIBILITY_DEFAULT QueryParserError : public RuntimeError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    QueryParserError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: RuntimeError(msg_, context_, "QueryParserError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit QueryParserError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: RuntimeError(msg_, context_, "QueryParserError", errno_) {}
    /** Construct from message and errno value. */
    QueryParserError(const std::string &msg_, int errno_)
	: RuntimeError(msg_, "", "QueryParserError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    QueryParserError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: RuntimeError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    QueryParserError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: RuntimeError(msg_, context_, type_, errno_) {}
};

/** RangeError indicates an attempt to access outside the bounds of a container.
 */
class XAPIAN_VISIBILITY_DEFAULT RangeError : public RuntimeError {
  public:
    /** @private @internal
     *  @brief Private constructor for use by remote backend.
     *
     *  @param error_string_	Optional string describing error.  May be NULL.
     */
    RangeError(const std::string &msg_, const std::string &context_, const char * error_string_)
	: RuntimeError(msg_, context_, "RangeError", error_string_) {}
    /** General purpose constructor which allows setting errno. */
    explicit RangeError(const std::string &msg_, const std::string &context_ = "", int errno_ = 0)
	: RuntimeError(msg_, context_, "RangeError", errno_) {}
    /** Construct from message and errno value. */
    RangeError(const std::string &msg_, int errno_)
	: RuntimeError(msg_, "", "RangeError", errno_) {}
  protected:
    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    RangeError(const std::string &msg_, const std::string &context_, const char * type_, const char * error_string_)
	: RuntimeError(msg_, context_, type_, error_string_) {}

    /** @private @internal
     *  @brief Constructor for use by constructors of derived classes.
     */
    RangeError(const std::string &msg_, const std::string &context_, const char * type_, int errno_)
	: RuntimeError(msg_, context_, type_, errno_) {}
};

}

#endif /* XAPIAN_INCLUDED_ERROR_H */
