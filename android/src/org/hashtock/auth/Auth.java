package org.hashtock.auth;

import java.io.IOException;

import android.os.Bundle;
import android.accounts.Account;
import android.accounts.AccountsException;
import android.accounts.AccountManager;
import android.accounts.AuthenticatorException;
import org.qtproject.qt5.android.bindings.QtActivity;

public class Auth extends QtActivity {
    private static AccountManager accountManager;

    public static String[] get_list()
    {
        Account[] accounts = accountManager.getAccountsByType("com.google");
        String[] names = new String[accounts.length];
        for (int i=0; i<accounts.length; i++) {
            names[i] = accounts[i].name;
        }
        return names;
    }

    public static String get_auth_key() //String username
    {
        String username = "mr.fuxi@gmail.com";
        Account account = getAccountForName(username);

        try {
            return getAuthToken(account);
        } catch (AccountsException e) {
            System.out.println("AccountsException");
            e.printStackTrace();
        } catch (IOException e) {
            System.out.println("IOException");
            e.printStackTrace();
        }

        return "";
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        accountManager = AccountManager.get(getApplicationContext());
    }

    private static String getAuthToken(Account account) throws AccountsException, IOException {
        // ToDo: This might need to start prompt screen or something
        return accountManager.blockingGetAuthToken(account, "ah", true);
    }

    private static Account getAccountForName(String username) {
        Account[] accounts = accountManager.getAccountsByType("com.google");
        if (accounts != null) {
            for (Account account : accounts) {
                if (account.name.equals(username)) {
                    return account;
                }
            }
        }
        return null;
    }
}
