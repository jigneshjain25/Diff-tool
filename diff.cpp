#include <bits/stdc++.h>
using namespace std;

#define MAXLINES 10001

int dp[MAXLINES][MAXLINES], penalty[MAXLINES][MAXLINES];
vector<string> v1,v2;


// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

vector<string> readFile(string fileName, bool trimLines = true) {
    vector<string> lines;
    string line;
    ifstream fin(fileName.c_str());
    if (fin.is_open()) {
        while(getline(fin, line)) {
            lines.push_back(line);
        }
        fin.close();
    }
    else {
        throw runtime_error("Could not open file: " + fileName);
    }

    return lines;
}

void writeToFile(vector<string> &lines, string fileName) {
    ofstream fout(fileName.c_str());
    if (fout.is_open()) {
        for(int i=0;i<lines.size();i++) {
            fout<<lines[i]<<endl;
        }

        fout.close();
    }
    else {
        throw runtime_error("Could not open file to write: " + fileName);
    }
}

void calculatePenalties() {
    for (int i=0;i<v1.size();i++) {
        string line1 = v1[i];
        trim(line1);
        for (int j=0;j<v2.size();j++) {
            string line2 = v2[j];
            trim(line2);
            penalty[i][j] = abs((int)line1.length() - (int)line2.length());
            for (int k=0;k<min(line1.length(), line2.length()); k++) {
                if (line1[k] != line2[k]) penalty[i][j]++;
            }
        }
    }
}

int solve(int i1, int i2) {
    int &ans = dp[i1][i2];
    if (ans == -1){
        if (i1 == v1.size()) {
            ans = 0;
            for (int i=i2;i<v2.size();i++)
                ans += (int)v2[i2].length();
        }
        else if (i2 == v2.size()) {
            ans = 0;
            for (int i=i1;i<v1.size();i++)
                ans += (int)v1[i1].length();
        }
        else {
            ans = penalty[i1][i2] + solve(i1+1,i2+1);
            ans = min(ans, (int)v1[i1].length() + solve(i1+1, i2));
            ans = min(ans, (int)v2[i2].length() + solve(i1, i2+1));
        }
    }

    //cout<<i1<<" "<<i2<<" "<<ans<<endl;
    return ans;
}

void backtrace(int i1, int i2) {
    if (i1 == v1.size()) {
        for (int i=i2;i<v2.size();i++)
            cout<<"+++ "<<v2[i]<<endl;
    }
    else if (i2 == v2.size()) {
        for (int i=i1;i<v1.size();i++)
            cout<<"--- "<<v1[i]<<endl;
    }
    else {
        int ans = dp[i1][i2];
        if (ans == penalty[i1][i2] + dp[i1+1][i2+1]) {
            if (penalty[i1][i2] > 0) {
                cout<<"*** "<<v1[i1]<<" -> "<<v2[i2]<<endl;
            }
            else {
                cout<<v1[i1]<<endl;
            }
            backtrace(i1+1,i2+1);
        }
        else if (ans == (int)v1[i1].length() + dp[i1+1][i2]) {
            cout<<"--- "<<v1[i1]<<endl;
            backtrace(i1+1,i2);
        }
        else if (ans == (int)v2[i2].length() + dp[i1][i2+1]) {
            cout<<"+++ "<<v2[i2]<<endl;
            backtrace(i1,i2+1);
        }
        else {
            throw runtime_error("this cant happen");
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 3){
        cout<<"Usage: diff.exe file1 file2";
        return 1;
    }

    try {
        memset(dp,-1,sizeof(dp));
        v1 = readFile(argv[1]);
        v2 = readFile(argv[2]);
        calculatePenalties();
        solve(0,0);
        backtrace(0,0);
    }
    catch(const exception &ex) {
        cout<<ex.what()<<endl;
    }
}