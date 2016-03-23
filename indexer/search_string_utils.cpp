#include "search_string_utils.hpp"

#include "std/set.hpp"
#include "std/transform_iterator.hpp"

#include "base/macros.hpp"

char const * STREET_TOKENS_SEPARATOR = "\t -,.";

strings::UniString search::FeatureTypeToString(uint32_t type)
{
  string const s = "!type:" + strings::to_string(type);
  return strings::UniString(s.begin(), s.end());
}

/// @todo Move prefixes, suffixes into separate file (autogenerated).
/// "Набережная" улица встречается в городах

char const * affics[] =
{
  // Russian
  "аллея", "бульвар", "набережная", "переулок", "площадь", "проезд", "проспект", "шоссе", "тупик", "улица", "тракт", "ал", "бул", "наб", "пер", "пл", "пр", "просп", "ш", "туп", "ул", "тр",

  // English
  "street", "avenue", "square", "road", "boulevard", "drive", "highway", "lane", "way", "st", "av", "ave", "sq", "rd", "blvd", "dr", "hwy", "ln",

  // German
  "strasse", "weg", "platz",

  // Lithuanian
  "g", "pr", "pl", "kel",

  // Български език - Bulgarian
  "булевард", "бул", "площад", "пл", "улица", "ул", "квартал", "кв",

  // Canada - Canada
  "allee", "alley", "autoroute", "aut", "bypass", "byway", "carrefour", "carref", "chemin", "côte", "crossing", "cross", "expressway", "freeway", "fwy", "line", "link", "loop", "parkway", "pky", "pkwy", "path", "pathway", "ptway", "route", "rte", "trail", "walk",

  // Cesky - Czech
  "ulice", "ul", "náměstí", "nám",

  // Deutsch - German
  "allee", "al", "brücke", "br", "chaussee", "gasse", "gr", "pfad", "straße", "str",

  // Español - Spanish
  "avenida", "avd", "avda", "bulevar", "bulev", "calle", "calleja", "cllja", "callejón", "callej", "cjon", "cllon", "callejuela", "cjla", "callizo", "cllzo", "calzada", "czada", "costera", "coste", "plza", "pza", "plazoleta", "pzta", "plazuela", "plzla", "tránsito", "trans", "transversal", "trval", "trasera", "tras", "travesía", "trva",

  // Français - French
  "rue", "avenue", "carré", "route", "boulevard", "drive", "autoroute", "lane", "chemin",

  // Nederlands - Dutch
  "laan", "ln.", "straat", "steenweg", "stwg", "st",

  // Norsk - Norwegian
  "vei", "veien", "vn", "gaten", "gata", "gt", "plass", "plassen", "sving", "svingen", "sv",

  // Polski - Polish
  "aleja", "aleje", "aleji", "alejach", "aleją", "plac", "placu", "placem", "ulica", "ulicy",

  // Português - Portuguese
  "street", "avenida", "quadrado", "estrada", "boulevard", "carro", "auto-estrada", "lane", "caminho",

  // Română - Romanian
  "bul", "bdul", "blv", "bulevard", "bulevardu", "calea", "cal", "piața", "pţa", "pța", "strada", "stra", "stradela", "sdla", "stradă", "unitate", "autostradă", "lane",

  // Slovenščina - Slovenian
  "cesta",

  // Suomi - Finnish
  "kaari", "kri", "katu", "kuja", "kj", "kylä", "polku", "tie", "t", "tori", "väylä", "vlä",

  // Svenska - Swedish
  "väg", "vägen", "gatan", "gränd", "gränden", "stig", "stigen", "plats", "platsen",

  // Türkçe - Turkish
  "sokak", "sk", "sok", "sokağı", "cadde", "cd", "caddesi", "bulvar", "bulvarı",

  // Tiếng Việt – Vietnamese
  "quốc lộ", "ql", "tỉnh lộ", "tl", "Đại lộ", "Đl", "Đường", "Đ", "Đường sắt", "Đs", "Đường phố", "Đp", "vuông", "con Đường", "Đại lộ", "Đường cao tốc",

  // Українська - Ukrainian
  "дорога", "провулок", "площа", "шосе", "вулиция", "дор", "пров", "вул"
};

void search::GetStreetName(strings::SimpleTokenizer iter, string & streetName)
{
  while (iter)
  {
    string const s = strings::MakeLowerCase(*iter);
    ++iter;

    char const ** end = affics + ARRAY_SIZE(affics);

    if (find(affics, end, s) == end)
      streetName += s;
  }
}

void search::GetStreetNameAsKey(string const & name, string & res)
{
  strings::SimpleTokenizer iter(name, STREET_TOKENS_SEPARATOR);
  GetStreetName(iter, res);
}

bool search::IsStreetSynonym(strings::UniString const & s)
{
  static set<strings::UniString> const kSynonyms(
      make_transform_iterator(affics, &search::NormalizeAndSimplifyString),
      make_transform_iterator(affics + ARRAY_SIZE(affics), &search::NormalizeAndSimplifyString));
  return kSynonyms.count(s) != 0;
}

bool search::ContainsNormalized(string const & str, string const & substr)
{
  strings::UniString const ustr = search::NormalizeAndSimplifyString(str);
  strings::UniString const usubstr = search::NormalizeAndSimplifyString(substr);
  return std::search(ustr.begin(), ustr.end(), usubstr.begin(), usubstr.end()) != ustr.end();
}
