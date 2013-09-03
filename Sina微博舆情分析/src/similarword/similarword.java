package similarword;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.net.URLEncoder;

import cache.cache;


public class similarword 
{
	public similarword() 
	{                                   //System.out.println("connect!!");
		// 1. 加载驱动程序
		try {
			Class.forName("com.mysql.jdbc.Driver");
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}// Mysql驱动

		// 2. 连续数据库
		try {
			conn = java.sql.DriverManager.getConnection(url, user, password);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	private java.sql.Connection conn;
	private static String url = "jdbc:mysql://127.0.0.1:3306/wordnet";
	private static String user = "root";//用户名	
	private static String password = "13071299";//密码

	public cache wordcache=new cache();//初始化cache
	public int finalverdict(String word1,String word2,String property)//0表示不是同义词  1表示是同义词  2表示无法判断的情况
		//该函数是将第一个词和第二个词互相判断是否为同义词，增加判定的准确性   。若这个函数低效，可直接用verdict函数执行一次判定也可。
	{                                       //System.out.println("finalverdict!!");
		if(wordcache.VerdictInCache(word1, word2)==1)return 1;
		String meaning=getmeanings(word1,property);
		int ans=verdict(meaning,word2,property);
		if(ans==1)wordcache.AddCacheitem(word1, word2);
		if(ans==1)return 1;
		meaning=getmeanings(word2,property);
		ans=verdict(meaning,word1,property);
		if(ans==1)wordcache.AddCacheitem(word1, word2);
		return ans;
	}//finalverdict
	public void disconnect()
	{                           //System.out.println("disconnect!!");
		//6 关闭数据库连接

		try {
			conn.close();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}//disconnect

	public int verdict(String meaning,String word2,String property)//0表示不是同义词  1表示是同义词  2表示无法判断的情况
		//判断两词是否为同义词，即判断第二个词是不是在第一个词的释义中。由于word net的同义词库是基于英文的，因此用这种方法也是迫不得已。
		//property表示词性，如a ad v n pron(代词) num art冠词      prep介词       conj连词      int感叹词     判断词yes和no   也可为null
	{                                                   //System.out.println("verdict!!");
		if(meaning=="/")return 2;//查词典无结果，则无法判断
		if(property==null)
		{
			Pattern patternOfWord = Pattern.compile(word2); //就是查找的单词
			Matcher m;
			m = patternOfWord.matcher(meaning);
			if(m.find())
				return 1;
			else
				return 0;
		}
		else
		{
			Pattern patternOfWord = Pattern.compile(property+"\\S+\\s+\\S+"+word2); //
			Matcher m;
			m = patternOfWord.matcher(meaning);
			if(m.find())
				return 1;
			else
				return 0;
		}
	}//verdict

	public String getmeanings(String originalword,String property)//获取所查词originalword的意思，其中包含了它的同义词
	{                                                        //System.out.println("getmeanings!!");
		/*
		// 1. 加载驱动程序
		try {
		Class.forName("com.mysql.jdbc.Driver");
		} catch (ClassNotFoundException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
		}// Mysql驱动

		// 2. 连续数据库
		java.sql.Connection conn;
		try {
		conn = java.sql.DriverManager.getConnection(url, user, password);
		} catch (SQLException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
		return "0";
		}*/

		//3  要执行的SQL语句
		Statement statement;
		try {
			statement = conn.createStatement();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "0";
		}
		/*
		   String org=null;
		   try {//这里尝试了各种编码方案
		   org=URLEncoder.encode(originalword, "utf-8");
		//org= new String(originalword.getBytes("GB2312"),"utf-8"); 
		org=originalword;
		} catch (UnsupportedEncodingException e1) {
		e1.printStackTrace();
		}*/
		String sql = "SELECT *FROM wn_synset WHERE synset_id=(SELECT e_syn_id FROM wn_map WHERE c_syn_id=(SELECT synset_id FROM wn_chinese WHERE chinese='"+originalword+"' LIMIT 1)LIMIT 1)";
		//String sql = "SELECT * FROM wn_synset WHERE word LIKE '%app%'";//test
		//System.out.println(sql);


		//4 结果
		ResultSet rs;
		try {
			rs = statement.executeQuery(sql);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "0";
		}
		//System.out.println("执行结果为：");

		String meaning = null;
		String ans="/";//返回的结果 

		try {
			while(rs.next()) {

				// 
				meaning = rs.getString("word_chs");

				//meaning = new String(meaning.getBytes("utf-8"),"utf-8");

				// System.out.println(meaning);
				ans+=meaning+"/";
			}

		} /*
		     catch (UnsupportedEncodingException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
		return "0";
		} */
		catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "0";
		}
		try {
			rs.close();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "0";
		}

		//5 像：'第二' '未必'这些词在wn_chinese里都没有  即数量词 代词和部分名词的同义词难以实现，因此在此做些弥补
		//if(ans.equals("/"))
		//{
		//若上一种查询没有结果，则采用这种模糊搜索的办法，因为是模糊的，该法可能不准确
		if(property==null)
			sql="SELECT *FROM wn_synset WHERE word_chs LIKE '%"+originalword+"%' LIMIT 5";
		else
			sql ="SELECT *FROM wn_synset WHERE word_chs LIKE '"+property+"%"+originalword+"%' LIMIT 5";
		//System.out.println(sql);
		try {
			rs = statement.executeQuery(sql);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "0";
		}
		//System.out.println("再执行结果为：");
		try {
			rs = statement.executeQuery(sql);
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "0";
		}

		meaning = null;

		try {
			while(rs.next()) {
				meaning = rs.getString("word_chs");
				// System.out.println(meaning);
				ans+=meaning+"/";
			}
		} 
		catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "0";
		}

		try {
			rs.close();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "0";
		}

		//}//if

		/*
		//6 关闭数据库连接

		try {
		conn.close();
		} catch (SQLException e) {
// TODO Auto-generated catch block
e.printStackTrace();
return "0";
}*/

//System.out.println(ans);
//System.out.println("fin");
return ans;
}//getmeanings

}
