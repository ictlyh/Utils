#include <fstream>
#include <iostream>
#include <limits.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

vector<string> line_split(string &str, char delim, int col_num = 0,
                          int col_idx = 0) {
  vector<string> cols;
  // Normal split by delim
  if (col_num == 0 && col_idx == 0) {
    stringstream ss(str);
    string item;
    while (getline(ss, item, delim)) {
      cols.push_back(item);
    }
    if (str[str.size() - 1] == delim)
      cols.push_back("");
    return cols;
  } else { // There are delims in col_idx'th col.
    vector<string> back_cols;
    if (col_idx > col_num || col_num < 1 || col_idx < 1) {
      cout << "Invalid col_num or col_idx" << endl;
      return cols;
    }
    size_t begin_idx = -1;
    size_t end_idx = 0;
    size_t idx; // start idx of special column
    for (int i = 1; i < col_idx; i++) {
      end_idx = str.find_first_of(delim, begin_idx + 1);
      cols.push_back(str.substr(begin_idx + 1, end_idx - begin_idx - 1));
      begin_idx = end_idx;
    }
    idx = begin_idx + 1;
    // end_idx = str.length() - 1 if there were semicolon in the end
    end_idx = str.length();
    for (int i = col_num; i > col_idx; i--) {
      begin_idx = str.find_last_of(delim, end_idx - 1);
      back_cols.push_back(str.substr(begin_idx + 1, end_idx - begin_idx - 1));
      end_idx = begin_idx;
    }
    string special = str.substr(idx, end_idx - idx);
    int delim_count = 0;
    for (size_t i = 0; i < special.length(); i++) {
      if (special[i] == delim)
        delim_count++;
    }
    char *buffer = new char[special.length() + delim_count + 1]();
    size_t j = special.length() + delim_count - 1;
    for (int i = special.length() - 1; i >= 0; i--) {
      buffer[j--] = special[i];
      if (special[i] == delim)
        buffer[j--] = '\\';
    }
    cols.push_back(string(buffer));
    delete[] buffer;
    for (int i = back_cols.size() - 1; i >= 0; i--)
      cols.push_back(back_cols[i]);
    return cols;
  }
}

string gen_line(vector<vector<string>> &records, char delim = ',') {
  stringstream ss;
  long record_num = records.size();
  int col_num = records[0].size();
  bool is_first = true;
  for (int i = 0; i < col_num; i++) {
    if (is_first)
      is_first = false;
    else
      ss << delim;
    ss << records[rand() % record_num][i];
  }
  return ss.str();
}

bool gen_file(string infile, string outfile, long line_of_source,
              long line_to_gen, char delim = ',', int col_num = 0,
              int col_idx = 0) {
  ifstream ifs(infile.c_str());
  if (!ifs.is_open()) {
    cout << "Unable to open file: " << infile;
    return false;
  }
  ofstream ofs(outfile.c_str());
  if (!ofs.is_open()) {
    cout << "Unable to write to file: " << outfile;
    ifs.close();
    return false;
  }
  vector<vector<string>> source_records;
  string line;
  long i = 0;
  while (i < line_of_source) {
    if (!getline(ifs, line)) {
      cout << "File terminated with " << i << " lines" << endl;
      ifs.close();
      line_of_source = i;
      break;
    }
    source_records.push_back(line_split(line, ',', col_num, col_idx));
    i++;
  }
  ifs.close();
  if (line_of_source == 0) {
    cout << "Source records number is 0, abort generating data." << endl;
    ofs.close();
    return false;
  }
  i = 0;
  while (i < line_to_gen) {
    line = gen_line(source_records);
    ofs << line << endl;
    i++;
  }
  ofs.close();
  return true;
}

string randomString(uint l = 15) {
  string charIndex(
      "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
  uint length = rand() % l + 1;
  uint ri[15];
  for (uint i = 0; i < length; ++i)
    ri[i] = rand() % charIndex.length();
  std::string rs = "";
  for (uint i = 0; i < length; ++i)
    rs += charIndex[ri[i]];
  if (rs.empty())
    return randomString(l);
  else
    return rs;
}

string gen_line(string types) {
  stringstream ss;
  for (unsigned int i = 0; i < types.length(); i++) {
    if (i != 0)
      ss << ",";
    if (types[i] == 'b' || types[i] == 'i') {
      int val = rand() % INT_MAX;
      ss << val;
    } else if (types[i] == 's')
      ss << randomString();
  }
  return ss.str();
}

bool gen_file(string outfile, long line_to_gen, string types) {
  long i = 0;
  ofstream ofs(outfile.c_str());
  if (!ofs.is_open()) {
    cout << "Can not write to file: " << outfile << endl;
    return false;
  }
  while (i < line_to_gen) {
    ofs << gen_line(types) << endl;
    i++;
  }
  ofs.close();
  return true;
}

int main() {
  srand(time(NULL));
  // 测试数据1
  // gen_file("url_rz.csv", "url_rz.out", 1500, 43000000000, ',', 19, 8);
  gen_file("url_rz.csv", "url_rz.out", 100, 10, ',', 19, 8);

  // 测试数据1 扩充
  // gen_file("url_rz.csv", "url_rz_extend.out", 1500, 56000000000, ',', 19, 8);
  gen_file("url_rz.csv", "url_rz_extend.out", 100, 10, ',', 19, 8);

  // 测试数据2
  string types("bsssiiiiissisissisisssiisiiiiibsbsbsbsiiib");
  // gen_file("zb_test.out", 1000000, types);
  gen_file("zb_test.out", 10, types);

  // 测试数据3
  // gen_file("RG_CUSTOMER_PEERIPDSTPACKET_M.csv",
  // "RG_CUSTOMER_PEERIPDSTPACKET_M.out", 1500, 4300000000);
  // gen_file("RG_CUSTOMER_PEERIPDSTPACKET_M.csv",
  // "RG_CUSTOMER_PEERIPDSTPACKET_M.out", 100, 10);

  // 测试数据4
  // gen_file("RG_CUSTOMER_DSTPORT_M.csv", "RG_CUSTOMER_DSTPORT_M.out", 1500,
  // 2600000000);
  gen_file("RG_CUSTOMER_DSTPORT_M.csv", "RG_CUSTOMER_DSTPORT_M.out", 100, 10);

  // 测试数据5
  // gen_file("RG_CUSTOMER_IPSRCBYTE_M.csv", "RG_CUSTOMER_IPSRCBYTE_M.out",
  // 1500, 1500000000);
  gen_file("RG_CUSTOMER_IPSRCBYTE_M.csv", "RG_CUSTOMER_IPSRCBYTE_M.out", 100,
           10);

  // 测试数据6
  types = "bssbbbisssbbbbss";
  // gen_file("comm_pkg.out", 130000000000, types);
  gen_file("comm_pkg.out", 10, types);

  // 测试数据7
  types = "bsssiiiiissisissisisssiisiiiiibsbsbsbsiiib";
  // gen_file("kgip_list.out", 2000000, types);
  gen_file("kgip_list.out", 10, types);

  // 测试数据8
  // gen_file("ip_list.out", 2000000, types);
  gen_file("ip_list.out", 10, types);

  // 测试数据9
  // gen_file("ip.out", 2000000, types);
  gen_file("ip.out", 10, types);

  // 测试数据10
  types = "bbsssiiiiissisissisisssiisiiiiibsbsbsbsiiib";
  // gen_file("list.out", 2000000, types);
  gen_file("list.out", 10, types);
  return 0;
}
