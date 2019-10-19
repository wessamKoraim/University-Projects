const int kMaxReferenceCount = 3;

class RestrictedRefCounter {
public:
	RestrictedRefCounter() : count_(1) {}
	
	bool Increment() {
		if(count_ <= kMaxReferenceCount) {
			count_++;
			return true;
		}
		return false;
	}
	
	int Decrement() { return --count_; }
	int GetCount() { return count_; }

private:
	int count_;
};
