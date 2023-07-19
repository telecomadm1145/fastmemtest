
namespace std
{
	class _Lockit {
	public:
		_Lockit() noexcept;
		explicit  _Lockit(int) noexcept;
		~_Lockit() noexcept;
	private:
		int _Locktype;
	};
	_Lockit::_Lockit() noexcept {}
	_Lockit::_Lockit(int) noexcept {}
	_Lockit::~_Lockit() noexcept {}
}