#pragma once

template <typename T, int E>
class Fifo {
public:
	static constexpr int maxsize = (1 << E) - 1;
	static constexpr unsigned char size_mask_ = maxsize;

	unsigned char size() const {
		return (tail_ - head_) & size_mask_;
	}

	bool empty() const {
		return tail_ == head_;
	}

	bool full() const {
		return size() == maxsize;
	}

	T& head() {
		return buffer_[head_];
	}

	T& tail() {
		return buffer_[tail_];
	}

	void pop() {
		head_ = (head_ + 1) & size_mask_;
	}

	void push() {
		tail_ = (tail_ + 1) & size_mask_;
	}

private:
	T buffer_[maxsize+1];

	volatile unsigned char head_ = 0; // old items are here
	volatile unsigned char tail_ = 0; // new items enter here
};
