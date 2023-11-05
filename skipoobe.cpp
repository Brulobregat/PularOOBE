/*
    netz - 06/11/2023
    use args "-static -static-libgcc -static-libstdc++" to compile
*/
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

bool IsRunningAsAdministrator() {
    BOOL isAdmin = FALSE;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID administratorsGroup;
    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup)) {
        if (!CheckTokenMembership(NULL, administratorsGroup, &isAdmin)) {
            isAdmin = FALSE;
        }
        FreeSid(administratorsGroup);
    }
    return isAdmin == TRUE;
}

int main() {
    system("cls");
    string usr;
    if (not(IsRunningAsAdministrator())) {
        cout << "Run this program in Administrator";
        return 0;
    }
    cout << "====== Microsoft OOBE Bypass ======" << "\n";
    cout << "Local Account - Enter your username: "; getline(cin, usr);
    if (usr.empty()) {
        usr = "Admin"; //Override if user doesn't input a name
    }
    // --- Actual payload ---

    // Registry fuckery in \SYSTEM\Setup
    system("reg.exe ADD HKLM\\SYSTEM\\Setup /v CmdLine /t REG_SZ /d \"\" /f");
    system("reg.exe ADD HKLM\\SYSTEM\\Setup /v RespecializeCmdLine /t REG_SZ /d \"\" /f");
    system("reg.exe ADD HKLM\\SYSTEM\\Setup /v OOBEInProgress /t REG_DWORD /d 0 /f");
    system("reg.exe ADD HKLM\\SYSTEM\\Setup /v SetupPhase /t REG_DWORD /d 0 /f");
    system("reg.exe ADD HKLM\\SYSTEM\\Setup /v SetupType /t REG_DWORD /d 0 /f");
    // Registry fuckery in \SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System
    system("reg.exe ADD HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System /v EnableCursorSurpession /t REG_DWORD /d 0 /f");
    // Add user
    string cmdl = "net user /add \"" + usr + "\"";
    char *usrn = &cmdl[0];
    system(usrn);
    cmdl = "net localgroup /add administrators \"" + usr + "\"";
    char *usrad = &cmdl[0];
    system(usrad);

    // -----------------------
    cout << "\n" << "Your computer will restart in 5 seconds" << "\n";
    system("shutdown /r /t 5");
    system("pause");
}