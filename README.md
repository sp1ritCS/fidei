<div style="text-align: center;" align="center">

![application icon](data/arpa.sp1rit.Fidei.svg)

<h1><b>Fidei</b></h1>
<p><i>Take back your faith</i></p>
<p>Bible reader for GNOME</p>
</div>

## Dependencies

| Library                                                        | pkg-config                     | Forge / Repository     |
|----------------------------------------------------------------|--------------------------------|------------|
| [glib2](https://wiki.gnome.org/Projects/GLib)                  | glib-2.0, gobject-2.0, gio-2.0 | [git@gitlab.gnome.org:GNOME/glib.git](https://gitlab.gnome.org/GNOME/glib/)             |
| [pango](https://www.pango.org/)                                | pango                          | [git@gitlab.gnome.org:GNOME/pango.git](https://gitlab.gnome.org/GNOME/pango/)           |
| [gtk4](https://www.gtk.org/)                                   | gtk4                           | [git@gitlab.gnome.org:GNOME/gtk.git](https://gitlab.gnome.org/GNOME/gtk/)               |
| [libadwaita](https://gnome.pages.gitlab.gnome.org/libadwaita/) | libadwaita-1                   | [git@gitlab.gnome.org:GNOME/libadwaita.git](https://gitlab.gnome.org/GNOME/libadwaita/) |
| [libxml2](https://xmlsoft.org)                                 | libxml-2.0                     | [git@gitlab.gnome.org:GNOME/libxml2.git](https://gitlab.gnome.org/GNOME/libxml2/)       |
| [libicu](https://icu.unicode.org)*                             | icu-uc                         | [git@github.com:unicode-org/icu.git](https://github.com/unicode-org/icu)                |

Optional dependencies are marked with \*

## Bible file-format

Bible files are XML files in [Zefania](https://www.bgfdb.de/zefaniaxml/bml/) format. While not strictly required, the bible file needs to contain the `<INFORMATION>` tag and provide the `bsname` attribute in `<BIBLEBOOK>`s to work propertly.

Fidei scans for bibles in `${XDG_DATA_HOME:-$HOME/.local/share}/arpa.sp1rit.Fidei` on app launch and makes them available in the bible picker (first view on first launch and later accesible in the menu).

A good source for such bibles is [bible4u.app](https://bible4u.app/download.html).
