package c45;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class C45 {

	class TreeNode {
		public TreeNode parent = null;//父节点
		public String decompositionAttribute = "";//当前节点分类属性
		public String pDecompositionValue = "";//父节点分类属性值
		public ArrayList<TreeNode> children = new ArrayList<TreeNode>();//字节点列表
		public String classLabel = "";//若当前节点为叶节点，则该节点表示的类别
	}

	private TreeNode treeRoot = null;


	private ArrayList<String> attributes = new ArrayList<String>();//存储属性名
	private ArrayList<ArrayList<String>> attributeValue = new ArrayList<ArrayList<String>>();//存储每个属性的取值
	private ArrayList<String[]> data = new ArrayList<String[]>();//存储String格式数据

	private int classAttributeIdx = -1;//分类属性在attributes列表中的索引

	//读取ARFF格式数据文件
	public void readARFF(String filename) {
		try {
			BufferedReader reader = new BufferedReader(new FileReader(filename));
			Pattern pattern = Pattern.compile("@attribute(.*)[{](.*?)[}]");
			String line = "";
			while ((line = reader.readLine()) != null) {
				Matcher matcher = pattern.matcher(line);
				if (matcher.find()) {
					attributes.add(matcher.group(1).trim());
					String[] values = matcher.group(2).split(",");;
					ArrayList<String> list = new ArrayList<String>(values.length);
					for (String value : values) {
						list.add(value.trim());
					}
					attributeValue.add(list);
				} else if (line.startsWith("@data")) {
					while ((line = reader.readLine()) != null) {
						if (line == "") {
							continue;
						}
						String[] row = line.split(",");
						data.add(row);
					}
				} else {
					continue;
				}
			}
			reader.close();
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	//打印数据，以确定数据读取是否正确
	public void printData() {
		System.out.println("@attributes");
		for (String attribute : attributes) {
			System.out.println(attribute);
		}
		System.out.println();

		System.out.println("@attributeValue");
		for (ArrayList<String> list : attributeValue) {
			for (String value : list) {
				System.out.print(value + " ");
			}
			System.out.println();
		}
		System.out.println();

		System.out.println("@data");
		for (String[] array : data) {
			for (String d : array) {
				System.out.print(d + " ");
			}
			System.out.println();
		}
		System.out.println();
	}

	//设置分类属性在attributes中的索引
	public void setClassAttributeIdx(int idx) {
		if (idx < 0 || idx >= attributes.size()) {
			System.err.println("class attribute set error!");
			System.exit(1);
		}
		classAttributeIdx = idx;
	}

	public void setClassAttribute(String classAttribute) {
		int n = attributes.indexOf(classAttribute);
		setClassAttributeIdx(n);
	}

	//判断subset中数据是否属于同一类
	public boolean classPure(ArrayList<Integer> subset) {
		String classLabel = data.get(subset.get(0))[classAttributeIdx];
		for (int i = 1; i < subset.size(); i++) {
			String nextClassLabel = data.get(subset.get(i))[classAttributeIdx];
			if (!nextClassLabel.equals(classLabel)) {
				return false;
			}
		}
		return true;
	}

	//统计不同类别计数
	public int[] classCount(ArrayList<Integer> subset) {
		ArrayList<String> attrval = attributeValue.get(classAttributeIdx);
		int[] count = new int[attrval.size()];
		for (int i = 0; i < subset.size(); i++) {
			String classLabel = data.get(subset.get(i))[classAttributeIdx];
			count[attrval.indexOf(classLabel)]++;
		}
		return count;
	}

	//多数表决判定结点类别
	public String MajorityVoting(ArrayList<Integer> subset) {
		int[] count = classCount(subset);

		int maxIdx = 0;
		for (int i = 1; i < count.length; i++) {
			if (count[i] > count[maxIdx]) {
				maxIdx = i;
			}
		}

		return attributeValue.get(classAttributeIdx).get(maxIdx);
	}

	//计算熵
	public double calEntropy(int[] count) {
		double entropy = 0.0;
		int total = 0;
		for (int num : count) {
			total += num;
		}

		for (int num : count) {
			if (num == 0 || total == 0) {
				return 0;
			}
			entropy += -(num * 1.0 / total) * Math.log(num * 1.0 / total) / Math.log(2); 
		}

		return entropy;
	}

	//计算信息增益
	public double calGainRatio(ArrayList<Integer> subset, int index) {
		//整体的熵
		double infoD = calEntropy(classCount(subset));

		//由属性index划分后的熵
		ArrayList<String> classattrval = attributeValue.get(classAttributeIdx);
		ArrayList<String> attrval = attributeValue.get(index);
		int[][] info = new int[attrval.size()][classattrval.size()];
		int[] count = new int[attrval.size()];
		for (int i = 0; i < subset.size(); i++) {
			int n = subset.get(i);
			int attrvalIndex = attrval.indexOf(data.get(n)[index]);
			int classattrvalIndex = classattrval.indexOf(data.get(n)[classAttributeIdx]);
			info[attrvalIndex][classattrvalIndex]++;
			count[attrvalIndex]++;
		}
		int sum = subset.size();
		double infoDA = 0.0;
		for (int i = 0; i < attrval.size(); i++) {
			infoDA += calEntropy(info[i]) * count[i] / sum;
		}

		return (infoD - infoDA) / infoD;
	}

	//构建分类决策树
	public TreeNode buildDecisionTree(LinkedList<Integer> selattr, ArrayList<Integer> subset, String pDecompositionValue) {
		TreeNode node = new TreeNode();
		node.pDecompositionValue = pDecompositionValue;

		//如果subset中所有数据都属于同一类
		if (classPure(subset)) {
			node.classLabel = data.get(subset.get(0))[classAttributeIdx];
			//System.out.println(node.pDecompositionValue + "\t" + node.classLabel);
			return node;
		} 

		//如果selattr候选分类属性集为空
		if (selattr.size() == 0) {
			node.classLabel = MajorityVoting(subset);//多数表决
			return node;
		}

		//计算各属性的信息增益，并从中选择信息增益最大的属性作为分类属性
		int maxIndex = -1;
		double maxEntropy = Double.MIN_VALUE;
		for (int i = 0; i < selattr.size(); i++) {
			double entropy = calGainRatio(subset, selattr.get(i));
			if (entropy > maxEntropy) {
				maxIndex = selattr.get(i);
				maxEntropy = entropy;
			}
		}
		//划分
		node.decompositionAttribute = attributes.get(maxIndex);
		selattr.remove(new Integer(maxIndex));
		ArrayList<String> attrval = attributeValue.get(maxIndex);
		for (String val : attrval) {
			ArrayList<Integer> subsubset = new ArrayList<Integer>();
			for (int i = 0; i < subset.size(); i++) {
				if (data.get(subset.get(i))[maxIndex].equals(val)) {
					subsubset.add(subset.get(i));
				}
			}
			if (subsubset.size() != 0) {
				TreeNode child = buildDecisionTree(selattr, subsubset, val);
				child.parent = node;
				node.children.add(child);
			} else {
				TreeNode child = new TreeNode();
				child.parent = node;
				child.classLabel = MajorityVoting(subset);
				node.children.add(child);
			}
		}

		return node;
	}


	// 需要参数：ARFF格式数据文件的文件名
	public static void main(String[] args) {
		if (args.length != 1) {
			System.err.println("need data file name!");
			System.exit(1);
		}

		C45 c45 = new C45();
		//读取ARFF格式数据文件
		c45.readARFF(args[0]);
		//c45.printData();
		c45.setClassAttribute("play");

		// 构建分类决策树
		LinkedList<Integer> selattr = new LinkedList<Integer>();//当前节点可用分类属性集
		for (int i = 0; i < c45.attributes.size(); i++) {
			if (i != c45.classAttributeIdx) {
				selattr.add(i);
			}
		}
		ArrayList<Integer> subset = new ArrayList<Integer>(c45.data.size());//当前节点训练数据集
		for (int i = 0; i < c45.data.size(); i++) {
			subset.add(i);
		}
		c45.treeRoot = c45.buildDecisionTree(selattr, subset, "");
		TreeNode temp = c45.treeRoot;
		while (temp.children.size() != 0) {
			System.out.println(temp.pDecompositionValue + "\t" + temp.decompositionAttribute);
			temp = temp.children.get(1);
		}
		System.out.println(temp.pDecompositionValue + "\t" + temp.decompositionAttribute);
		System.out.println(temp.classLabel);
	}
}
