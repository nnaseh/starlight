#include <catch2/catch_test_macros.hpp>
#include "services/clipboard/clipboard-source.hpp"

using namespace Clipboard;

// NOLINTBEGIN(bugprone-throwing-static-initialization)
namespace {
const MenuOwner PASSWORDS_EXTRA{.bundleId = "com.apple.Passwords.MenuBarExtra",
                                .appBundleId = "com.apple.Passwords",
                                .isMenuBarExtra = true};
} // namespace

TEST_CASE("declared pasteboard source wins") {
  auto a = attributeSource(QString("com.example.Declared"), QString("com.apple.Safari"), PASSWORDS_EXTRA);
  REQUIRE(a.sourceApp == "com.example.Declared");
  REQUIRE(a.relatedApps.empty());
}

TEST_CASE("frontmost app is used without an open menu bar extra") {
  auto a = attributeSource(std::nullopt, QString("com.apple.Safari"), std::nullopt);
  REQUIRE(a.sourceApp == "com.apple.Safari");
  REQUIRE(a.relatedApps.empty());

  MenuOwner regular = PASSWORDS_EXTRA;
  regular.isMenuBarExtra = false;
  a = attributeSource(std::nullopt, QString("com.apple.Safari"), regular);
  REQUIRE(a.sourceApp == "com.apple.Safari");
}

TEST_CASE("menu bar extra copies are credited to the owning app") {
  auto a = attributeSource(std::nullopt, QString("com.apple.Safari"), PASSWORDS_EXTRA);
  REQUIRE(a.sourceApp == "com.apple.Passwords");
  REQUIRE(a.relatedApps == std::vector<QString>{"com.apple.Passwords.MenuBarExtra", "com.apple.Safari"});
}

TEST_CASE("excluding the app, its helper or the frontmost app drops the copy") {
  auto a = attributeSource(std::nullopt, QString("com.apple.Terminal"), PASSWORDS_EXTRA);

  REQUIRE(isFromIgnoredApp(a.sourceApp, a.relatedApps, {"com.apple.Passwords"}));
  REQUIRE(isFromIgnoredApp(a.sourceApp, a.relatedApps, {"com.apple.Passwords.MenuBarExtra"}));
  REQUIRE(isFromIgnoredApp(a.sourceApp, a.relatedApps, {"com.apple.Terminal"}));
  REQUIRE_FALSE(isFromIgnoredApp(a.sourceApp, a.relatedApps, {"com.apple.Safari"}));
  REQUIRE_FALSE(isFromIgnoredApp(std::nullopt, {}, {"com.apple.Safari"}));
}
// NOLINTEND(bugprone-throwing-static-initialization)
