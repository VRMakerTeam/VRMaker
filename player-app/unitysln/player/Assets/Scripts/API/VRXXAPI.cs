using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using SimpleJSON;

public class VRXXAPI : MonoBehaviour {


    public static VRXXAPI instance;

    public delegate void onWXUserInfoResultDelegate(string user);

    public onWXUserInfoResultDelegate onWXUserInfoResult;


    public class UserInfo
    {
        public string nickname;
        public string sex;
        public string headimgurl;
    }

    class Access_Token
    {
        public string acces_token;
        public string refresh_token;
        public string openid;
    }

    private const string app_id = "wxa0679c5d46e06efe";
    private const string app_secret = "da39c7ea545a96280481207ea83d44ee";
    private static Access_Token token = new Access_Token();

    void Start()
    {
        instance = this;
    }

    public void WXAuthCallback(string _code)
    {
        string url = "https://api.weixin.qq.com/sns/oauth2/access_token?appid=" + app_id + "&secret=" + app_secret + "&code=" + _code.Trim() + "&grant_type=authorization_code";
        StartCoroutine(getToken(url, () => {}));
    }

    private void GetUserInfo()
    {
        string url = "https://api.weixin.qq.com/sns/userinfo?access_token=" + token.acces_token + "&openid=" + token.openid;
        StartCoroutine(getUserInfo(url));
    }

    private IEnumerator getToken(string _url, System.Action _onSuccess)
    {
        WWW www = new WWW(_url);
        yield return www;
        if (www.error != null)
        {
            Debug.Log("error is : " + www.error);
        }
        else
        {
            JSONNode root = JSON.Parse(www.text);
            token.acces_token = root["access_token"].Value.Trim();
            token.openid = root["openid"].Value.Trim();
            GetUserInfo();
        }
    }

    private IEnumerator getUserInfo(string _url)
    {
        WWW www = new WWW(_url);
        yield return www;
        if (www.error != null)
        {
            Debug.Log("VRXXAPI error is : " + www.error);
        }
        else
        {
            if (this.onWXUserInfoResult != null)
                onWXUserInfoResult(www.text);
        }
    }

}
