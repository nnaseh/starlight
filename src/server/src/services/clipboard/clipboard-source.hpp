#pragma once
#include <QString>
#include <algorithm>
#include <optional>
#include <string>
#include <vector>

namespace Clipboard {

/**
 * The process owning a menu that is on screen while a copy happens.
 */
struct MenuOwner {
  QString bundleId;
  /**
   * The outermost app bundle containing the process, e.g Passwords.app for its embedded menu bar
   * helper. Same as `bundleId` when the process is not embedded in another app.
   */
  QString appBundleId;
  /**
   * Whether the process is a background agent (menu bar extra) rather than a regular app.
   */
  bool isMenuBarExtra = false;
};

struct SourceAttribution {
  std::optional<QString> sourceApp;
  /**
   * Other apps that may have made the copy. The selection is ignored if any of them is excluded.
   */
  std::vector<QString> relatedApps;
};

/**
 * Menu bar extras never become frontmost, so copies made from their menus would otherwise be credited
 * to whatever app is in front. We credit the extra's owning app instead, but keep the helper and the
 * frontmost app as related apps so that excluding any of them still drops the copy.
 */
inline SourceAttribution attributeSource(const std::optional<QString> &declared,
                                         const std::optional<QString> &frontmost,
                                         const std::optional<MenuOwner> &menu) {
  if (declared) return {.sourceApp = declared, .relatedApps = {}};
  if (!menu || !menu->isMenuBarExtra || menu->appBundleId.isEmpty()) {
    return {.sourceApp = frontmost, .relatedApps = {}};
  }

  SourceAttribution attribution{.sourceApp = menu->appBundleId, .relatedApps = {}};
  if (menu->bundleId != menu->appBundleId) attribution.relatedApps.emplace_back(menu->bundleId);
  if (frontmost && *frontmost != menu->appBundleId) attribution.relatedApps.emplace_back(*frontmost);

  return attribution;
}

inline bool isFromIgnoredApp(const std::optional<QString> &sourceApp, const std::vector<QString> &relatedApps,
                             const std::vector<std::string> &ignoredApps) {
  auto ignored = [&](const QString &id) { return std::ranges::contains(ignoredApps, id.toStdString()); };
  return (sourceApp && ignored(*sourceApp)) || std::ranges::any_of(relatedApps, ignored);
}

} // namespace Clipboard
