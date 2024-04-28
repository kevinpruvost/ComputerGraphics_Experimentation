#pragma once

#include <functional>
#include <memory>

template<typename ReturnType = void, typename... Args>
using CallbackContainer = std::function<ReturnType(Args...)>;

template<typename ReturnType = void, typename... Args>
class Callback : public std::unique_ptr<CallbackContainer<ReturnType, Args...>>
{
public:
	Callback(const CallbackContainer<ReturnType, Args...>& callback)
		: std::unique_ptr<CallbackContainer<ReturnType, Args...>>(callback != nullptr ? new CallbackContainer<ReturnType, Args...>(callback) : nullptr)
	{
	}
	Callback& operator=(const CallbackContainer<ReturnType, Args...>& callback)
	{
		this->Reset(callback);
		return *this;
	}
	~Callback() = default;
	ReturnType operator()(Args... args)
	{
		return (*(*this))(args...);
	}
	void inline Reset(const CallbackContainer<ReturnType, Args...>& callback)
	{
        this->reset(new CallbackContainer<ReturnType, Args...>(callback));
    }

private:

};