package main;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import similarword.similarword;

import dataStructure.SinaWeibo;
import dataStructure.TextSeg;
import ICTCLAS.I3S.AC.ICTCLAS50;
import weibo.Weibo;
import vector.vector;

public class WeiboMain {

	public static void main(String[] args) {
		//Weibo weibo=new Weibo();
		//weibo.init();
		vector vt = new vector();
		System.out.println("begin");
		ArrayList<Byte[]> vectorlist = (ArrayList<Byte[]>)vt.getVectorlist();
		System.out.println('\n');

		for( Byte[] bt:vectorlist ){
			for( int i = 0; i < bt.length; i++ ){
				System.out.print(bt[i] + " ");		
			}
			System.out.println('\n');	
		}

		System.out.println("end");
	}

	//similarword供实验用
	public static void similarword() {
		//String word1="谈话",word2="说话",property="v";
		String word1="高兴", word2="开心", property=null;//cache do have
		//String word1="伤心",word2="悲伤",property=null;
		similarword similar = new similarword();

		//调用的示例
		//以下函数的最后一个变量为property，表示词性，如a ad v n pron(代词) num art冠词      prep介词       conj连词      int感叹词     判断词yes和no，也可为null
		//1
		int ans = similar.finalverdict(word1, word2, property);

		if(ans == 1) {
			System.out.println(word1+" 和 " + word2 + "是同义词");		
		} else {
			if(ans == 0) {
				System.out.println(word1 + " 和 " + word2 + "不是同义词");
			} else {
				System.out.println("无法判断 " + word1 + "和 " + word2 + "是同义词");//也可认为不是同义词
			}
		}

		//2
		word1 = "健康";//cache do not have
		word2 = "健壮";
		property = null;
		ans=similar.finalverdict(word1, word2, property);

		if(ans == 1) {
			System.out.println(word1 + " 和 " + word2 + "是同义词");		
		} else {
			if(ans == 0) {
				System.out.println(word1 + " 和 " + word2 + "不是同义词");
			} else {
				System.out.println("无法判断 " + word1 + "和 " + word2 + "是同义词");//也可认为不是同义词
			}
		}

		//3
		word1 = "计算"; //cache do not have
		word2 = "计数";
		property = null;
		ans=similar.finalverdict(word1, word2, property);
		if(ans == 1) {
			System.out.println(word1 + " 和 " + word2 + "是同义词");		
		} else {
			if(ans == 0)
				System.out.println(word1 + " 和 " + word2 + "不是同义词");
			else
				System.out.println("无法判断 " + word1 + "和 " + word2 + "是同义词");//也可认为不是同义词
		}

		//4
		word1 = "指望";    //cache do not have
		word2 = "依赖";
		property = null;
		ans=similar.finalverdict(word1, word2, property);
		if(ans == 1) {
			System.out.println(word1 + " 和 " + word2 + "是同义词");		
		} else {
			if(ans == 0) {
				System.out.println(word1 + " 和 " + word2 + "不是同义词");
			} else {
				System.out.println("无法判断 " + word1 + "和 " + word2 + "是同义词");//也可认为不是同义词
			}
		}

		//5
		word1 = "猪八戒照镜子";   //cache have (long word)
		word2 = "思考";
		property = null;
		ans = similar.finalverdict(word1, word2, property);
		if(ans == 1) {
			System.out.println(word1 + " 和 " + word2 + "是同义词");		
		} else {
			if(ans == 0)
				System.out.println(word1 + " 和 " + word2 + "不是同义词");
			else
				System.out.println("无法判断 " + word1 + "和 " + word2 + "是同义词");//也可认为不是同义词
		}

		//6
		word1 = "春哥";//neither cache nor database have
		word2 = "纯爷们";
		property = null;
		ans = similar.finalverdict(word1,word2,property);

		if(ans == 1) {
			System.out.println(word1 + " 和 " + word2 + "是同义词");		
		} else {
			if(ans == 0) {
				System.out.println(word1 + " 和 " + word2 + "不是同义词");
			} else {
				System.out.println("无法判断 " + word1 + "和 " + word2 + "是同义词");//也可认为不是同义词
			}
		}

		similar.disconnect();//不用之后关闭数据库连接
		similar.wordcache.rewrite();//退出程序不再使用cache时，写回文件中保存
	}
}
