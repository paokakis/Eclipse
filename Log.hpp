#ifndef LOG_H
#define LOG_H

class Log {
public:
	static void info(const char*, ...);
	static void warning(const char*, ...);
	static void error(const char*, ...);
private:
	explicit Log() {};
	~Log() {};
};

#endif // !LOG_H
