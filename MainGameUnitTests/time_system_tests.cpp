// ReSharper disable once CppUnusedIncludeDirective
#include "../MainGame/time_system.cpp"
#include "../MainGame/time_system.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace mr;

namespace main_game_unit_tests
{
	TEST_CLASS(time_system_tests)
	{
	public:

		TEST_METHOD(init_given_start_time_initializes_time)
		{
			// Arrange
			time_system time_sys;

			// Act
			time_sys.init(1.0, { 12, 12, 12 });
			const auto result = time_sys.get_time_total_micro_seconds();

			// Assert
			Assert::AreEqual(43932000000, result);
		}

		TEST_METHOD(update_given_faster_ratio_updates_time)
		{
			// Arrange
			time_system time_sys;
			time_sys.init(1.0 / 2.0, { 0, 0, 1 });

			// Act
			time_sys.interact(long long(2e6));
			const auto result = time_sys.get_time_total_micro_seconds();

			// Assert
			// Time flows two times FASTER, so four seconds should pass
			Assert::AreEqual(long long(5e6), result);
		}

		TEST_METHOD(update_given_slower_ratio_updates_time)
		{
			// Arrange
			time_system time_sys;
			time_sys.init(2.0 / 1.0, { 0, 0, 1 });

			// Act
			time_sys.interact(long long(2e6));
			const auto result = time_sys.get_time_total_micro_seconds();

			// Assert
			// Time flows two times SLOWER, so only one second should pass
			Assert::AreEqual(long long(2e6), result);
		}

		TEST_METHOD(update_given_end_of_the_day_starts_new_day)
		{
			// Arrange
			time_system time_sys;
			time_sys.init(1.0, { 23, 59, 59 });

			// Act
			time_sys.interact(long long(1e6));
			const auto result = time_sys.get_time_total_micro_seconds();

			// Assert
			Assert::AreEqual(0LL, result);
		}

		TEST_METHOD(get_time_normalized_given_time_returns_normalized_time)
		{
			// Arrange
			time_system time_sys;
			time_sys.init(1.0, { 12, 0, 0 });

			// Act
			const auto result = time_sys.get_time_normalized();

			// Assert
			Assert::AreEqual(0.5F, result);
		}
	};
}
