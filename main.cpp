#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>

class Date {
 private:
  int day, month, year;
  static bool IsRight(int d, int m, int y) {
    if (y > 0) {
      if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 ||
          m == 12) {
        if (d > 0 && d <= 31) {
          return true;
        }
      }
      if (m == 4 || m == 6 || m == 9 || m == 11) {
        if (d > 0 && d <= 30) {
          return true;
        }
      }
      if (m == 2) {
        if (d > 0 && d <= 28) {
          return true;
        } else if (d == 29) {
          if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
            return true;
          } else {
            return false;
          }
        }
        return false;
      }
    }
    return false;
  }

  void Check(int d, int m, int y) {
    if (IsRight(d, m, y)) {
      day = d;
      month = m;
      year = y;
    } else {
      day = 1;
      month = 1;
      year = 1970;
    }
  }

  bool LongYear(int y) const {
    if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
      return true;
    }
    return false;
  }

  int Days(int m, int y) const {
    if (m == 4 || m == 6 || m == 9 || m == 11) {
      return 30;
    } else if (m == 2) {
      if (LongYear(y)) {
        return 29;
      }
      return 28;
    } else {
      return 31;
    }
  }

  long long ToSeconds(int d, int m, int y) const {
    long long seconds = 0;

    for (int i = 1; i < y + 1; ++i) {
      if (LongYear(i)) {
        seconds += 60 * 60 * 24 * 366;
      } else {
        seconds += 60 * 60 * 24 * 365;
      }
    }

    for (int i = 1; i < m; ++i) {
      seconds += Days(i, y) * 60 * 60 * 24;
    }

    seconds += 60 * 60 * 24 * (d - 1);

    return seconds;
  }

 public:
  Date(int d, int m, int y) { Check(d, m, y); }

  int GetDay() const { return day; }

  int GetMonth() const { return month; }

  int GetYear() const { return year; }

  int DiffDays(const Date& other) const {
    int64_t seconds1 = ToSeconds(day, month, year);
    int64_t seconds2 =
        ToSeconds(other.GetDay(), other.GetMonth(), other.GetYear());

    return (std::abs(int(seconds1 - seconds2))) / (60 * 60 * 24);
  }

  void add(int days) {
    int64_t seconds = ToSeconds(day, month, year);
    seconds += days * 60 * 60 * 24;

    int64_t total_days = seconds / (60 * 60 * 24);

    int y = 1;
    int d = LongYear(y) ? 366 : 365;
    while (total_days > d) {
      total_days -= d;
      y++;
      d = LongYear(y) ? 366 : 365;
    }

    int m = 1;
    while (total_days > Days(m, y)) {
      total_days -= Days(m, y);
      m++;
    }

    day = total_days;
    month = m;
    year = y;
  }

  void subtract(int days) {
    int64_t seconds = ToSeconds(day, month, year);
    seconds -= days * 60 * 60 * 24;

    int64_t total_days = seconds / (60 * 60 * 24);

    int y = 0;
    int d = LongYear(y) ? 366 : 365;
    while (total_days > d) {
      total_days -= d;
      y++;
      d = LongYear(y) ? 366 : 365;
    }

    int m = 1;
    while (total_days > Days(m, y)) {
      total_days -= Days(m, y);
      m++;
    }

    day = total_days + 1;
    month = m;
    year = y;
  }

  Date replace(int new_day = 0, int new_month = 0, int new_year = 0) const {
    int d = (new_day == 0) ? day : new_day;
    int m = (new_month == 0) ? month : new_month;
    int y = (new_year == 0) ? year : new_year;

    return Date(d, m, y);
  }

  int week() const {
    int64_t seconds = ToSeconds(day, month, year);
    return int(seconds / 60 / 60 / 24 / 7);
  }

  int totalWeeks() {
    int64_t seconds1 = ToSeconds(day, month, year);
    int64_t seconds2 = ToSeconds(1, 1, 1970);

    return int(seconds1 - seconds2) / (60 * 60 * 24 * 7);
  }

  char* isoformat() const {
    std::string iso_str = std::to_string(year) + "-";
    if (month < 10) {
      iso_str += "0";
    }
    iso_str += std::to_string(month) + "-";
    if (day < 10) {
      iso_str += "0";
    }
    iso_str += std::to_string(day);

    char* iso_cstr = new char[iso_str.size() + 1];
    std::strcpy(iso_cstr, iso_str.c_str());
    return iso_cstr;
  }
};