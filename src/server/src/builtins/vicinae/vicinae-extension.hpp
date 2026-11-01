#include "command/command-database.hpp"
#include "ui/image/url.hpp"
#include "vicinae.hpp"
#include <QCoreApplication>
#include <qurlquery.h>

class VicinaeExtension : public BuiltinCommandRepository {
  QString id() const override { return "core"; }
  QString displayName() const override { return "Starlight"; }
  QString description() const override {
    return QCoreApplication::translate("VicinaeExtension", "General Starlight-related commands.");
  }
  ImageURL iconUrl() const override {
    return ImageURL::builtin(BuiltinIcon::Vicinae).setBackgroundTint(Omnicast::ACCENT_COLOR);
  }

public:
  VicinaeExtension();
};
