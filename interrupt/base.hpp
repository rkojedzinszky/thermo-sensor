#pragma once

typedef void (*callback_t)();

template <int Interrupt_num>
class Interrupt {
public:
	static void set(callback_t callback);
	static void clear();
	static void pending();
	static void tick();
private:
	static volatile bool fire_;
	static callback_t callback_;
};

template <int Interrupt_num>
inline void Interrupt<Interrupt_num>::set(callback_t callback)
{
	fire_ = false;
	callback_ = callback;
}

template <int Interrupt_num>
inline void Interrupt<Interrupt_num>::clear()
{
	set(nullptr);
}

template <int Interrupt_num>
inline void Interrupt<Interrupt_num>::pending()
{
	while (fire_) {
		fire_ = false;
		callback_();
	}
}

template <int Interrupt_num>
inline void Interrupt<Interrupt_num>::tick()
{
	if (callback_ != nullptr) {
		fire_ = true;
	}
}
