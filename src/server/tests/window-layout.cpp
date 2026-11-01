#include <catch2/catch_test_macros.hpp>
#include "builtins/wm/window-layout.hpp"

// NOLINTBEGIN(bugprone-throwing-static-initialization)
namespace {
bool same(const WindowBounds &a, const WindowBounds &b) {
  return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
}
} // namespace

TEST_CASE("halves split the available area without gaps") {
  const QRect area{0, 25, 1001, 775};
  const WindowBounds win{.x = 10, .y = 40, .width = 300, .height = 200};

  const auto left = layoutBounds(WindowLayout::LeftHalf, area, win);
  const auto right = layoutBounds(WindowLayout::RightHalf, area, win);
  REQUIRE(same(left, {.x = 0, .y = 25, .width = 500, .height = 775}));
  REQUIRE(same(right, {.x = 500, .y = 25, .width = 501, .height = 775}));

  const auto top = layoutBounds(WindowLayout::TopHalf, area, win);
  const auto bottom = layoutBounds(WindowLayout::BottomHalf, area, win);
  REQUIRE(same(top, {.x = 0, .y = 25, .width = 1001, .height = 387}));
  REQUIRE(same(bottom, {.x = 0, .y = 412, .width = 1001, .height = 388}));
}

TEST_CASE("thirds cover the full width") {
  const QRect area{100, 0, 1000, 600};
  const WindowBounds win{};

  REQUIRE(same(layoutBounds(WindowLayout::LeftThird, area, win),
               {.x = 100, .y = 0, .width = 333, .height = 600}));
  REQUIRE(same(layoutBounds(WindowLayout::CenterThird, area, win),
               {.x = 433, .y = 0, .width = 333, .height = 600}));
  REQUIRE(same(layoutBounds(WindowLayout::RightThird, area, win),
               {.x = 766, .y = 0, .width = 334, .height = 600}));
}

TEST_CASE("maximize and center") {
  const QRect area{0, 25, 1000, 775};

  REQUIRE(
      same(layoutBounds(WindowLayout::Maximize, area, {}), {.x = 0, .y = 25, .width = 1000, .height = 775}));

  const WindowBounds win{.x = 0, .y = 0, .width = 400, .height = 300};
  REQUIRE(
      same(layoutBounds(WindowLayout::Center, area, win), {.x = 300, .y = 262, .width = 400, .height = 300}));

  const WindowBounds huge{.x = 0, .y = 0, .width = 4000, .height = 3000};
  REQUIRE(
      same(layoutBounds(WindowLayout::Center, area, huge), {.x = 0, .y = 25, .width = 1000, .height = 775}));
}

TEST_CASE("moving to another area keeps relative position and size") {
  const QRect from{0, 0, 1000, 800};
  const QRect to{1000, 0, 2000, 1600};
  const WindowBounds win{.x = 500, .y = 400, .width = 500, .height = 400};

  REQUIRE(same(moveToArea(win, from, to), {.x = 2000, .y = 800, .width = 1000, .height = 800}));
}
// NOLINTEND(bugprone-throwing-static-initialization)
