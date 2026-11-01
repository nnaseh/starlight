#pragma once
#include <algorithm>
#include <QRect>
#include "services/window-manager/abstract-window-manager.hpp"

namespace WindowSnap {

enum class WindowLayout {
  LeftHalf,
  RightHalf,
  TopHalf,
  BottomHalf,
  LeftThird,
  CenterThird,
  RightThird,
  Maximize,
  Center,
};

using WindowBounds = AbstractWindowManager::WindowBounds;

inline WindowBounds layoutBounds(WindowLayout layout, const QRect &area, const WindowBounds &current) {
  const int x = area.x();
  const int y = area.y();
  const int w = area.width();
  const int h = area.height();
  const int third = w / 3;

  switch (layout) {
  case WindowLayout::LeftHalf:
    return {.x = x, .y = y, .width = w / 2, .height = h};
  case WindowLayout::RightHalf:
    return {.x = x + w / 2, .y = y, .width = w - w / 2, .height = h};
  case WindowLayout::TopHalf:
    return {.x = x, .y = y, .width = w, .height = h / 2};
  case WindowLayout::BottomHalf:
    return {.x = x, .y = y + h / 2, .width = w, .height = h - h / 2};
  case WindowLayout::LeftThird:
    return {.x = x, .y = y, .width = third, .height = h};
  case WindowLayout::CenterThird:
    return {.x = x + third, .y = y, .width = third, .height = h};
  case WindowLayout::RightThird:
    return {.x = x + 2 * third, .y = y, .width = w - 2 * third, .height = h};
  case WindowLayout::Maximize:
    return {.x = x, .y = y, .width = w, .height = h};
  case WindowLayout::Center: {
    const int cw = std::min(current.width, w);
    const int ch = std::min(current.height, h);
    return {.x = x + (w - cw) / 2, .y = y + (h - ch) / 2, .width = cw, .height = ch};
  }
  }

  return current;
}

/**
 * Move a window from one screen area to another, keeping its relative position and size.
 */
inline WindowBounds moveToArea(const WindowBounds &current, const QRect &from, const QRect &to) {
  if (from.width() <= 0 || from.height() <= 0) return current;

  auto scaleX = [&](int v) {
    return static_cast<int>(static_cast<long long>(v) * to.width() / from.width());
  };
  auto scaleY = [&](int v) {
    return static_cast<int>(static_cast<long long>(v) * to.height() / from.height());
  };

  return {.x = to.x() + scaleX(current.x - from.x()),
          .y = to.y() + scaleY(current.y - from.y()),
          .width = std::min(scaleX(current.width), to.width()),
          .height = std::min(scaleY(current.height), to.height())};
}

} // namespace WindowSnap
