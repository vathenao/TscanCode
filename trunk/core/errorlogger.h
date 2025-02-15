/*
 * TscanCode - A tool for static C/C++ code analysis
 * Copyright (C) 2017 TscanCode team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//---------------------------------------------------------------------------
#ifndef errorloggerH
#define errorloggerH
//---------------------------------------------------------------------------

#include <list>
#include <string>

#include "config.h"
#include "suppressions.h"
#include "gitblamemgr.h"

class Token;
class TokenList;
class Scope;
/// @addtogroup Core
/// @{

/** @brief Simple container to be thrown when internal error is detected. */
struct InternalError {
    enum Type {SYNTAX, INTERNAL};
    InternalError(const Token *tok, const std::string &errorMsg, Type type = INTERNAL);
    const Token *token;
    std::string errorMessage;
    std::string id;
};

/** @brief enum class for severity. Used when reporting errors. */
class TSCANCODELIB Severity {
public:
    /**
     * Message severities.
     */
    enum SeverityType {
        /**
         * No severity (default value).
         */
        none,
        /**
         * Programming error.
         * This indicates severe error like memory leak etc.
         * The error is certain.
         */
        error,
        /**
         * Warning.
         * Used for dangerous coding style that can cause severe runtime errors.
         * For example: forgetting to initialize a member variable in a constructor.
         */
        warning,
        /**
         * Style warning.
         * Used for general code cleanup recommendations. Fixing these
         * will not fix any bugs but will make the code easier to maintain.
         * For example: redundant code, unreachable code, etc.
         */
        style,
        /**
         * Performance warning.
         * Not an error as is but suboptimal code and fixing it probably leads
         * to faster performance of the compiled code.
         */
        performance,
        /**
         * Portability warning.
         * This warning indicates the code is not properly portable for
         * different platforms and bitnesses (32/64 bit). If the code is meant
         * to compile in different platforms and bitnesses these warnings
         * should be fixed.
         */
        portability,
        /**
         * Checking information.
         * Information message about the checking (process) itself. These
         * messages inform about header files not found etc issues that are
         * not errors in the code but something user needs to know.
         */
        information,
        /**
         * Debug message.
         * Debug-mode message useful for the developers.
         */
        debug
    };

    static std::string toString(SeverityType severity) {
        switch (severity) {
        case none:
            return "";
        case error:
            return "error";
        case warning:
            return "warning";
        case style:
            return "style";
        case performance:
            return "performance";
        case portability:
            return "portability";
        case information:
            return "information";
        case debug:
            return "debug";
        };
        throw InternalError(NULL, "Unknown severity");
    }
    static SeverityType fromString(const std::string &severity) {
        if (severity.empty())
            return none;
        if (severity == "none")
            return none;
        if (severity == "error")
            return error;
        if (severity == "warning")
            return warning;
        if (severity == "style")
            return style;
        if (severity == "performance")
            return performance;
        if (severity == "portability")
            return portability;
        if (severity == "information")
            return information;
        if (severity == "debug")
            return debug;
        return none;
    }
};

class TSCANCODELIB ErrorType
{
public:
	enum ErrorTypeEnum
	{
		None,
		Logic,
		Suspicious,
		Compute,
		NullPointer,
		BufferOverrun,
		MemoryLeak,
		UnsafeFunc,
		Unity,
		UserCustom,
		Uninit
	};

	static std::string ToString(ErrorTypeEnum type)
	{
		switch (type)
		{
		case ErrorType::None:
			return "none";
		case ErrorType::Logic:
			return "logic";
		case ErrorType::Suspicious:
			return "suspicious";
		case ErrorType::Compute:
			return "compute";
		case ErrorType::NullPointer:
			return "nullpointer";
		case ErrorType::BufferOverrun:
			return "bufoverrun";
		case ErrorType::MemoryLeak:
			return "memleak";
		case ErrorType::UnsafeFunc:
			return "unsafefunc";
		case ErrorType::Unity:
			return "unity";
		case ErrorType::UserCustom:
			return "custom";
		case ErrorType::Uninit:
			return "uninit";
		default:
			return "none";
		}
	}
};

/**
 * @brief This is an interface, which the class responsible of error logging
 * should implement.
 */
class TSCANCODELIB ErrorLogger {
public:

    /**
     * Wrapper for error messages, provided by reportErr()
     */
    class TSCANCODELIB ErrorMessage {
    public:
        /**
         * File name and line number.
         * Internally paths are stored with / separator. When getting the filename
         * it is by default converted to native separators.
         */
        class TSCANCODELIB FileLocation {
        public:
            FileLocation()
                : line(0) {
            }

            FileLocation(const std::string &file, unsigned int aline)
                : line(aline), _file(file) {
            }

            FileLocation(const Token* tok, const TokenList* list);

            /**
             * Return the filename.
             * @param convert If true convert path to native separators.
             * @return filename.
             */
            std::string getfile(bool convert = true) const;

            /**
             * Set the filename.
             * @param file Filename to set.
             */
            void setfile(const std::string &file);

            /**
             * @return the location as a string. Format: [file:line]
             */
            std::string stringify() const;

            unsigned int line;
        private:
            std::string  _file;
        };

		ErrorMessage(const std::list<FileLocation> &callStack, Severity::SeverityType severity, const std::string &msg, ErrorType::ErrorTypeEnum type, const std::string& id, bool inconclusive);
		ErrorMessage(const std::list<const Token*>& callstack, const TokenList* list, Severity::SeverityType severity, ErrorType::ErrorTypeEnum type, const std::string& id, const std::string& msg, bool inconclusive);
		ErrorMessage();

		/**
		* Format the error message in XML format
		* @param verbose use verbose message
		* @param ver XML version
		*/
		std::string toXML(bool verbose, int ver) const;

        /**
         * Format the error message in XML format
         * @param verbose use verbose message
         * @param ver XML version
         */
		std::string toXML_codetrace(bool verbose, int ver) const;

        static std::string getXMLHeader(int xml_version);
        static std::string getXMLFooter(int xml_version);

        /**
         * Format the error message into a string.
         * @param verbose use verbose message
         * @param outputFormat Empty string to use default output format
         * or template to be used. E.g. "{file}:{line},{severity},{id},{message}"
        * @return formatted string
         */
        std::string toString(bool verbose, const std::string &outputFormat = emptyString) const;

        std::string serialize() const;
        bool deserialize(const std::string &data);

        std::list<FileLocation> _callStack;
		ErrorType::ErrorTypeEnum _type;
		std::string _id;

        /** source file (not header) */
        std::string file0;

        Severity::SeverityType _severity;
        unsigned int _cwe;
        bool _inconclusive;
		std::string _webIdentify;
		std::string _funcinfo;

        /** set short and verbose messages */
        void setmsg(const std::string &msg);

        /** Short message (single line short message) */
        const std::string &shortMessage() const {
            return _shortMessage;
        }

        /** Verbose message (may be the same as the short message) */
        const std::string &verboseMessage() const {
            return _verboseMessage;
        }

		void SetWebIdentity(const std::string &identity) { _webIdentify = identity; }

		void SetFuncInfo(const std::string& funcInfo) { _funcinfo = funcInfo; }

private:
    void gitBlame(const std::string& fullName, const unsigned int line) const;

private:
        /**
         * Replace all occurrences of searchFor with replaceWith in the
         * given source.
         * @param source The string to modify
         * @param searchFor What should be searched for
         * @param replaceWith What will replace the found item
         */
        static void findAndReplace(std::string &source, const std::string &searchFor, const std::string &replaceWith);

        static std::string fixInvalidChars(const std::string& raw);

        /** Short message */
        std::string _shortMessage;

        /** Verbose message */
        std::string _verboseMessage;
		std::unique_ptr<GitBlameParser> m_gitParser;
    };

    ErrorLogger() { }
    virtual ~ErrorLogger() { }

    /**
     * Information about progress is directed here.
     * Override this to receive the progress messages.
     *
     * @param outmsg Message to show e.g. "Checking main.cpp..."
     */
    virtual void reportOut(const std::string &outmsg) = 0;

    /**
     * Information about found errors and warnings is directed
     * here. Override this to receive the errormessages.
     *
     * @param msg Location and other information about the found error.
     */
    virtual void reportErr(const ErrorLogger::ErrorMessage &msg) = 0;

	virtual void reportErr(const std::string &errmsg){ }

    /**
     * Report progress to client
     * @param filename main file that is checked
     * @param stage for example preprocess / tokenize / simplify / check
     * @param value progress value (0-100)
     */
    virtual void reportProgress(const std::string &filename, const char stage[], const std::size_t value) {
        (void)filename;
        (void)stage;
        (void)value;
    }

    /**
     * Output information messages.
     * @param msg Location and other information about the found error.
     */
    virtual void reportInfo(const ErrorLogger::ErrorMessage &msg) {
        reportErr(msg);
    }

    /**
     * Report list of unmatched suppressions
     * @param unmatched list of unmatched suppressions (from Settings::Suppressions::getUnmatched(Local|Global)Suppressions)
     */
    void reportUnmatchedSuppressions(const std::list<Suppressions::SuppressionEntry> &unmatched);

    static std::string callStackToString(const std::list<ErrorLogger::ErrorMessage::FileLocation> &callStack);

	static const std::string GenWebIdentity(const std::string &idenity, const std::string &retfrom);
	static const std::string GenWebIdentity(const std::string &idenity);
	static void GetScopeFuncInfo(const Scope* s, std::string &_lastFuncName);
};

/// @}
//---------------------------------------------------------------------------
#endif // errorloggerH
