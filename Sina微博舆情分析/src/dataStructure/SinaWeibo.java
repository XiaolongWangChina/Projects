package dataStructure;

import java.util.ArrayList;

public class SinaWeibo {

	private String docno = null;
	private String userid = null;
	private String text = null; 
	private ArrayList<TextSeg> textSeg=null;//¿¿¿¿¿¿

	public SinaWeibo(String docno,String userid,String text,ArrayList<TextSeg> textSeg)
	{
		this.docno = docno;
		this.userid = userid;
		this.text = text;
		this.textSeg = textSeg;
	}


	public String getDocno() {
		return docno;
	}

	public void setDocno(String docno) {
		this.docno = docno;
	}

	public String getUserid() {
		return userid;
	}

	public void setUserid(String userid) {
		this.userid = userid;
	}

	public String getText() {
		return text;
	}

	public void setText(String text) {
		this.text = text;
	}

	public ArrayList<TextSeg> getTextSeg() {
		return textSeg;
	}

	public void setTextSeg(ArrayList<TextSeg> textSeg) {
		this.textSeg = textSeg;
	}

}
