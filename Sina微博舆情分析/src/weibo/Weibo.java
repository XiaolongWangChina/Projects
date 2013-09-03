package weibo;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

import ICTCLAS.I3S.AC.ICTCLAS50;

import dataStructure.*;

public class Weibo {

	private ArrayList<SinaWeibo> sinaWeibo; //载入好的微博数据
	HashMap<String,Integer> hsMap=new HashMap<String,Integer>();

	public ArrayList<SinaWeibo> getSinaWeibo() {
		return this.sinaWeibo;
	}
	public HashMap<String,Integer> gethsMap() {
		return this.hsMap;
	}
	public void setSinaWeibo(ArrayList<SinaWeibo> sw){
		this.sinaWeibo=sw;
	}

	ICTCLAS50 testICTCLAS50;

	public Weibo() {
	}

	public void init() {

		testICTCLAS50 = new ICTCLAS50();
		// 分词所需库的路径
		String argu = ".";
		this.hsMap.clear();
		// 初始化ICTCLAS
		try {
			if (testICTCLAS50.ICTCLAS_Init(argu.getBytes("GB2312")) == false) {
				System.out.println("Init fail!");
				return;
			} else {
				System.out.println("Init ICTCLAS succeed.");
			}
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}


		ArrayList<SinaWeibo> sinaWeibo = new ArrayList<SinaWeibo>();

		//读入微博数据
		Scanner in = null;
		int count = 0;
		try {
			in = new Scanner(new FileInputStream("data\\test_sinaweibo.txt"),
					"UTF-8");
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block

			e.printStackTrace();
			System.out.println("Fail to read weibo-text.");
			System.out.println("Init failed.");
		}


		if (in != null) {

			String temp_docno = null;
			String temp_userid = null;
			String temp_text = null;
			ArrayList<TextSeg> temp_textSeg = new ArrayList<TextSeg>();

			String line = in.nextLine();

			//找到第一条数据
			while (!line.equals("<DOC>") && in.hasNext()) {
				line = in.nextLine();
			}

			//当还有数据时
			while (in.hasNext()) {

				//读入一条微博
				if (line.startsWith("<DOC>") && line.endsWith("<DOC>")) {
					line = in.nextLine();
				}
				if (line.startsWith("<DOCNO>") && line.endsWith("</DOCNO>")) {
					temp_docno = line.substring(7, line.length() - 8);
					line = in.nextLine();
				}
				if (line.startsWith("<USER>") && line.endsWith("</USER>")) {
					temp_userid = line.substring(6, line.length() - 7);
					line = in.nextLine();
				}
				if (line.startsWith("<TEXT>") && line.endsWith("</TEXT>")) {
					temp_text = line.substring(6, line.length() - 7);
					line = in.nextLine();
				}
				// 分词
				try {
					byte nativeBytes[] = testICTCLAS50
						.ICTCLAS_ParagraphProcess(
								temp_text.getBytes("GB2312"), 0, 1);
					String nativeStr = new String(nativeBytes, 0,
							nativeBytes.length, "GB2312");

					//分词后，将分的词与其词性一一对应并保存					
					temp_textSeg = new ArrayList<TextSeg>();
					while (nativeStr.length() != 1
							&& (nativeStr.substring(0, 1).startsWith(" ") || nativeStr
								.substring(0, 1).charAt(0) == '\t')) {
						nativeStr = nativeStr.substring(1);
					}
					//分词之后是一个分好的词加上一个/，后面是他的词性，加上一个空格，若想观察分词的形式，可以加个语句打印nativeStr
					while (nativeStr.length() != 1) {
						int i = nativeStr.indexOf("/", 1);
						int j = nativeStr.indexOf(" ", 1);

						if (i < 0 || j < 0 || i > j) {
							//这个是用来发现那些无法进行分词的
							System.out.println(nativeStr);
							nativeStr = " ";
							System.out.println(count);
						} else {
							//对成功分词的保存结果
							//System.out.println(nativeStr);
							/*temp_textSeg.add(new TextSeg(nativeStr.substring(0,
							  i), nativeStr.substring(i + 1, j)));
							  nativeStr = nativeStr.substring(j);
							  while (nativeStr.length() != 1
							  && (nativeStr.substring(0, 1).startsWith(
							  " ") || nativeStr.substring(0, 1)
							  .charAt(0) == '\t')) {
							  nativeStr = nativeStr.substring(1);
							  }*/
							String nativeStrPOS=nativeStr.substring(i + 1, j);
							if(nativeStrPOS.startsWith("n")/*||nativeStrPOS.startsWith("a")*/)

							{

								//System.out.println(nativeStrPOS);
								temp_textSeg.add(new TextSeg(nativeStr.substring(0,
												i), nativeStr.substring(i + 1, j)));
								if(this.hsMap.containsKey(nativeStr.substring(0,i))){
									Integer n=this.hsMap.get(nativeStr.substring(0,i));
									this.hsMap.put(nativeStr.substring(0,i), n+1);
								}
								else
									this.hsMap.put(nativeStr.substring(0,i), 1);
							}
							nativeStr = nativeStr.substring(j);
							while (nativeStr.length() != 1
									&& (nativeStr.substring(0, 1).startsWith(
											" ") || nativeStr.substring(0, 1)
										.charAt(0) == '\t')) {
								nativeStr = nativeStr.substring(1);
							}
						}

					}

				} catch (UnsupportedEncodingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				//
				if (line.startsWith("</DOC>") && line.endsWith("</DOC>")) {
					line = in.nextLine();
				}

				//加入分词后结果
				sinaWeibo.add(new SinaWeibo(null, null, null,
							temp_textSeg));
				count += 1;
				System.out.println(count);


				//寻找下一条微博数据
				while (!line.startsWith("<DOC>") && in.hasNext()) {
					line = in.nextLine();
				}

			}

		}
		this.sinaWeibo=sinaWeibo;
		System.out.println("Load " + count + " sinaWeibo.");
		System.out.println("Init succeed");

		testICTCLAS50.ICTCLAS_Exit();

	}
}
