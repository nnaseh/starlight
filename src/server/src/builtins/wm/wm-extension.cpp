#include "wm-extension.hpp"
#include "services/builtin-icon/builtin-icon.hpp"
#include "service-registry.hpp"
#include "services/window-manager/abstract-window-manager.hpp"
#include "services/window-manager/window-manager.hpp"
#include "services/toast/toast-service.hpp"
#include "builtins/wm/switch-windows-view-host.hpp"
#include "builtins/wm/switch-workspaces-view-host.hpp"
#include "builtins/wm/window-layout.hpp"
#include "ui/image/url.hpp"
#include "command/single-view-command-context.hpp"
#include "theme/theme.hpp"
#include <qcontainerfwd.h>
#include "theme/colors.hpp"
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>
#include <expected>

namespace {
using namespace WindowSnap;

const auto COLOR = SemanticColor::Blue;

class ToggleFullscreenWindowCommand : public BuiltinCallbackCommand {
  QString id() const override { return "toggle-fullscreen"; }
  QString name() const override {
    return QCoreApplication::translate("ToggleFullscreenWindowCommand", "Toggle Fullscreen");
  }
  ImageURL iconUrl() const override {
    return ImageURL::builtin(BuiltinIcon::Fullscreen).setBackgroundTint(COLOR);
  }
  void execute(CommandController &ctrl) const override {
    auto wm = ctrl.context()->services->windowManager();
    auto toast = ctrl.context()->services->toastService();

    if (auto window = wm->provider()->getFocusedWindowSync()) {
      if (wm->isOnActiveWorkspace(*window)) {
        wm->provider()->toggleFullscreen(*window);
      } else {
        toast->failure(QCoreApplication::translate("ToggleFullscreenWindowCommand",
                                                   "Active window is not on the current workspace"));
      }
      ctrl.context()->navigation->closeWindow();
    } else {
      toast->failure(QCoreApplication::translate("ToggleFullscreenWindowCommand", "No window to fullscreen"));
    }
  }
};

class ToggleFloatingWindowCommand : public BuiltinCallbackCommand {
  QString id() const override { return "toggle-floating"; }
  QString name() const override {
    return QCoreApplication::translate("ToggleFloatingWindowCommand", "Toggle Floating");
  }
  ImageURL iconUrl() const override {
    return ImageURL::builtin(BuiltinIcon::FloatingWindow).setBackgroundTint(COLOR);
  }
  void execute(CommandController &ctrl) const override {
    auto wm = ctrl.context()->services->windowManager();
    auto toast = ctrl.context()->services->toastService();

    if (auto window = wm->provider()->getFocusedWindowSync()) {
      if (wm->isOnActiveWorkspace(*window)) {
        wm->provider()->toggleFloating(*window);
      } else {
        toast->failure(QCoreApplication::translate("ToggleFloatingWindowCommand",
                                                   "Active window is not on the current workspace"));
      }
      ctrl.context()->navigation->closeWindow();
    } else {
      toast->failure(QCoreApplication::translate("ToggleFloatingWindowCommand", "No window to toggle"));
    }
  }
};

class ToggleOverviewCommand : public BuiltinCallbackCommand {
  QString id() const override { return "toggle-overview"; }
  QString name() const override {
    return QCoreApplication::translate("ToggleOverviewCommand", "Toggle Overview");
  }
  ImageURL iconUrl() const override {
    return ImageURL::builtin(BuiltinIcon::Overview).setBackgroundTint(COLOR);
  }
  void execute(CommandController &controller) const override {
    auto wm = controller.context()->services->windowManager();
    wm->provider()->toggleOverview();
    controller.context()->navigation->closeWindow();
  }
};

struct LayoutCommandInfo {
  const char *id;
  const char *name;
  BuiltinIcon icon;
};

constexpr LayoutCommandInfo layoutCommandInfo(WindowLayout layout) {
  switch (layout) {
  case WindowLayout::LeftHalf:
    return {"left-half", QT_TRANSLATE_NOOP("WindowLayoutCommand", "Left Half"),
            BuiltinIcon::AppWindowSidebarLeft};
  case WindowLayout::RightHalf:
    return {"right-half", QT_TRANSLATE_NOOP("WindowLayoutCommand", "Right Half"),
            BuiltinIcon::AppWindowSidebarRight};
  case WindowLayout::TopHalf:
    return {"top-half", QT_TRANSLATE_NOOP("WindowLayoutCommand", "Top Half"), BuiltinIcon::AppWindow};
  case WindowLayout::BottomHalf:
    return {"bottom-half", QT_TRANSLATE_NOOP("WindowLayoutCommand", "Bottom Half"), BuiltinIcon::AppWindow};
  case WindowLayout::LeftThird:
    return {"left-third", QT_TRANSLATE_NOOP("WindowLayoutCommand", "Left Third"),
            BuiltinIcon::AppWindowGrid3x3};
  case WindowLayout::CenterThird:
    return {"center-third", QT_TRANSLATE_NOOP("WindowLayoutCommand", "Center Third"),
            BuiltinIcon::AppWindowGrid3x3};
  case WindowLayout::RightThird:
    return {"right-third", QT_TRANSLATE_NOOP("WindowLayoutCommand", "Right Third"),
            BuiltinIcon::AppWindowGrid3x3};
  case WindowLayout::Maximize:
    return {"maximize", QT_TRANSLATE_NOOP("WindowLayoutCommand", "Maximize Window"), BuiltinIcon::Maximize};
  case WindowLayout::Center:
    return {"center", QT_TRANSLATE_NOOP("WindowLayoutCommand", "Center Window"), BuiltinIcon::Center};
  }
  return {"", "", BuiltinIcon::AppWindow};
}

QScreen *screenForBounds(const WindowBounds &bounds) {
  QPoint const center{bounds.x + bounds.width / 2, bounds.y + bounds.height / 2};
  if (auto screen = QGuiApplication::screenAt(center)) return screen;
  return QGuiApplication::primaryScreen();
}

/**
 * Run `fn` on the focused window, its current bounds and the screen it is on, then close the launcher.
 * `fn` returns the new bounds, or a message to show when there is nothing to do.
 */
template <typename Fn> void withFocusedWindowBounds(CommandController &ctrl, Fn &&fn) {
  auto wm = ctrl.context()->services->windowManager();
  auto toast = ctrl.context()->services->toastService();
  auto window = wm->provider()->getFocusedWindowSync();
  auto bounds = window ? window->bounds() : std::nullopt;

  if (!bounds) {
    toast->failure(QCoreApplication::translate("WindowLayoutCommand", "No window to move"));
    return;
  }

  QScreen *screen = screenForBounds(*bounds);

  if (!screen) {
    toast->failure(QCoreApplication::translate("WindowLayoutCommand", "No display found"));
    return;
  }

  std::expected<WindowBounds, QString> target = fn(*bounds, screen);

  if (!target) {
    toast->failure(target.error());
    return;
  }

  if (!wm->provider()->setWindowBounds(*window, *target)) {
    toast->failure(QCoreApplication::translate("WindowLayoutCommand", "Failed to move window"));
    return;
  }

  ctrl.context()->navigation->closeWindow();
}

template <WindowLayout L> class WindowLayoutCommand : public BuiltinCallbackCommand {
  static constexpr LayoutCommandInfo INFO = layoutCommandInfo(L);

  QString id() const override { return INFO.id; }
  QString name() const override { return QCoreApplication::translate("WindowLayoutCommand", INFO.name); }
  ImageURL iconUrl() const override { return ImageURL::builtin(INFO.icon).setBackgroundTint(COLOR); }
  void execute(CommandController &ctrl) const override {
    withFocusedWindowBounds(
        ctrl, [](const WindowBounds &bounds, QScreen *screen) -> std::expected<WindowBounds, QString> {
          return layoutBounds(L, screen->availableGeometry(), bounds);
        });
  }
};

class NextDisplayCommand : public BuiltinCallbackCommand {
  QString id() const override { return "next-display"; }
  QString name() const override {
    return QCoreApplication::translate("NextDisplayCommand", "Move Window to Next Display");
  }
  ImageURL iconUrl() const override {
    return ImageURL::builtin(BuiltinIcon::Monitor).setBackgroundTint(COLOR);
  }
  void execute(CommandController &ctrl) const override {
    withFocusedWindowBounds(
        ctrl, [](const WindowBounds &bounds, QScreen *current) -> std::expected<WindowBounds, QString> {
          auto screens = QGuiApplication::screens();
          if (screens.size() < 2) {
            return std::unexpected(
                QCoreApplication::translate("NextDisplayCommand", "Only one display connected"));
          }
          auto idx = screens.indexOf(current);
          auto next = screens.at((idx + 1) % screens.size());
          return moveToArea(bounds, current->availableGeometry(), next->availableGeometry());
        });
  }
};

class SwitchWindowsCommand : public BuiltinViewCommand<SwitchWindowsViewHost> {
  QString id() const override { return "switch-windows"; }
  QString name() const override {
    return QCoreApplication::translate("SwitchWindowsCommand", "Switch Windows");
  }
  ImageURL iconUrl() const override {
    return ImageURL::builtin(BuiltinIcon::SwitchWindows).setBackgroundTint(COLOR);
  }
};

class SwitchWorkspacesCommand : public BuiltinViewCommand<SwitchWorkspacesViewHost> {
  QString id() const override { return "switch-workspaces"; }
  QString name() const override {
#ifdef Q_OS_WIN
    return QCoreApplication::translate("SwitchWorkspacesCommand", "Switch Desktops");
#else
    return QCoreApplication::translate("SwitchWorkspacesCommand", "Switch Workspaces");
#endif
  }
  std::vector<QString> keywords() const override {
    return {"workspaces", "desktops", "virtual desktops", "spaces"};
  }
  ImageURL iconUrl() const override {
    return ImageURL::builtin(BuiltinIcon::Carousel).setBackgroundTint(COLOR);
  }
};
} // namespace

WindowManagementExtension::WindowManagementExtension(const ServiceRegistry &services) {
  using Cap = AbstractWindowManager::Capability;
  auto wm = services.windowManager()->provider();

  registerCommand<SwitchWindowsCommand>();

  if (wm->hasWorkspaces()) { registerCommand<SwitchWorkspacesCommand>(); }
  if (wm->supports(Cap::Fullscreen)) { registerCommand<ToggleFullscreenWindowCommand>(); }
  if (wm->supports(Cap::ToggleFloating)) { registerCommand<ToggleFloatingWindowCommand>(); }
  if (wm->supports(Cap::ToggleOverview)) { registerCommand<ToggleOverviewCommand>(); }
  if (wm->supports(AbstractWindowManager::Capability::SetSticky)) {}
  if (wm->supports(Cap::SetBounds)) {
    registerCommand<WindowLayoutCommand<WindowLayout::LeftHalf>>();
    registerCommand<WindowLayoutCommand<WindowLayout::RightHalf>>();
    registerCommand<WindowLayoutCommand<WindowLayout::TopHalf>>();
    registerCommand<WindowLayoutCommand<WindowLayout::BottomHalf>>();
    registerCommand<WindowLayoutCommand<WindowLayout::LeftThird>>();
    registerCommand<WindowLayoutCommand<WindowLayout::CenterThird>>();
    registerCommand<WindowLayoutCommand<WindowLayout::RightThird>>();
    registerCommand<WindowLayoutCommand<WindowLayout::Maximize>>();
    registerCommand<WindowLayoutCommand<WindowLayout::Center>>();
    registerCommand<NextDisplayCommand>();
  }
}

QString WindowManagementExtension::id() const { return "wm"; }

QString WindowManagementExtension::displayName() const {
  return QCoreApplication::translate("WindowManagementExtension", "Window Management");
}
ImageURL WindowManagementExtension::iconUrl() const {
  return ImageURL::builtin(BuiltinIcon::AppWindow).setBackgroundTint(COLOR);
}
