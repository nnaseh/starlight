<div>
  <p align="center">
    <img
      width="96"
      src="extra/starlight.png"
      alt="Starlight logo"
    />
  </p>
  
  <h1 align="center"><b>Starlight</b></h1>
  <h4 align="center">A focused launcher for your desktop</h4>
  
  <p align="center">
     <a href="https://vicinae.com"><img src="https://img.shields.io/badge/website-vicinae.com-black.svg" alt="Website"></a>
    <a href="https://discord.gg/rP4ecD42p7"><img src="https://img.shields.io/badge/Discord-Join-5865F2?logo=discord&logoColor=white" alt="Discord"></a>
  </p>
  <img alt="Vicinae screenshot" src="extra/screenshot.png" />
</div>


> [!NOTE]
> **Starlight is a personal fork of [Vicinae](https://github.com/vicinaehq/vicinae)**, rebranded for macOS.
> All of the launcher itself is the work of the Vicinae authors and contributors. Starlight is distributed
> under the same [GPL-3.0 license](LICENSE), and upstream copyright notices are kept as is. The rest of
> this README is the upstream Vicinae README.
>
> **Starlight replaces Vicinae, it is not meant to run next to it.** Before installing Starlight:
>
> 1. Quit Vicinae (menu bar icon, then Quit Vicinae).
> 2. Uninstall it with `brew uninstall --cask vicinae` (do not pass `--zap`, which would delete your settings).
>    If you installed it from the DMG, move `/Applications/Vicinae.app` to the Trash instead.
> 3. Install and open Starlight.app.
>
> Your settings, extensions, clipboard history and other data carry over, because Starlight keeps the same
> config and data directories (`~/.config/vicinae`, `~/.local/share/vicinae`, `~/.local/state/vicinae`).
> macOS treats Starlight as a new app, so you have to grant Accessibility and other permissions again.
> Starlight uses its own runtime directory and IPC socket (`$TMPDIR/starlight/starlight.sock`), so it never
> talks to or replaces a Vicinae instance that is still running. The `vicinae` command line tool, the
> `vicinae://` URL scheme and the browser extension's native messaging host (`com.vicinae.vicinae`) keep their
> names so existing scripts, extensions and the browser extension keep working.

**Vicinae** (pronounced _"vee-CHEE-nay"_) is a high-performance, native command palette for your desktop.

Out of the box, Vicinae can be your:

- app search
- [clipboard history](https://docs.vicinae.com/clipboard)
- [text expander (snippets)](https://docs.vicinae.com/snippets)
- [file search](https://docs.vicinae.com/file-search)
- [browser tab switcher](https://docs.vicinae.com/browser-extension)
- [emoji picker](https://docs.vicinae.com/emoji)
- [calculator](https://docs.vicinae.com/calculator)
- [window/workspace switcher](https://docs.vicinae.com/window)
- font browser
- volume controller

When you need more, Vicinae can be extended in several ways:

- [React/Typescript extensions](https://docs.vicinae.com/extensions/introduction), compatible with the Raycast ecosystem. In-app integration with the [Vicinae store](https://www.vicinae.com/extensions) and the [Raycast store](https://www.raycast.com/store).
- [Script commands](https://docs.vicinae.com/scripts/getting-started), also compatible with the Raycast feature of the same name, with special Vicinae additions.
- [dmenu style menu creation](https://docs.vicinae.com/dmenu), the linux minimalist way!

## Documentation

Everything you need to know to get started with Vicinae is at [vicinae.com](https://vicinae.com). If you have any question, please feel free to come ask it on [discord](https://discord.gg/rP4ecD42p7)!

## Sponsors

### Gold sponsors

<table>
  <tbody>
    <tr>
      <td align="center" valign="middle">
        <a href="https://depot.dev/?utm_source=vicinae&utm_medium=readme" target="_blank">
          <picture>
            <source media="(prefers-color-scheme: light)" srcset="https://depot.dev/assets/brand/horizontal/v2/depot-logo-horizontal-on-light-x3.png" />
            <source media="(prefers-color-scheme: dark)" srcset="https://depot.dev/assets/brand/horizontal/v2/depot-logo-horizontal-on-dark-x3.png" />
            <img src="https://depot.dev/assets/brand/horizontal/v2/depot-logo-horizontal-on-light-x3.png" width="450" alt="Depot logo" />
          </picture>
        </a>
      </td>
    </tr>
  </tbody>
</table>

### Silver Sponsors

<table>
  <tbody>
    <tr>
      <td align="center" valign="middle">
        <a href="https://coderabbit.link/vicinaehq" target="_blank"><img src="https://avatars.githubusercontent.com/u/132028505?s=200&v=4" height="100" alt="CodeRabbit logo"></a>
      </td>
    </tr>
  </tbody>
</table>

### Supporter sponsors

<table>
  <tbody>
    <tr>
      <td align="center" valign="middle">
        <a href="https://github.com/jaredallard" target="_blank"><img src="https://avatars.githubusercontent.com/u/2391349?v=4" height="60" alt="Jared Allard's pp"></a>
      </td>
    </tr>
  </tbody>
</table>

---

[And all our other amazing sponsors!](https://github.com/sponsors/vicinaehq#sponsors)

## Acknowledgments

Special thanks to the [Soulver](https://soulver.app?utm_source=vicinae&utm_medium=readme) team for allowing us to ship SoulverCore as a calculator backend option on macOS.
