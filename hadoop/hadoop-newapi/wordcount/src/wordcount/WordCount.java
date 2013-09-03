package wordcount;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class WordCount {

	// mapper class
	public static class TokenizerMapper
		extends Mapper<Object, Text, Text, IntWritable> {
			private final static IntWritable one = new IntWritable(1);
			private Text word = new Text();
			private IntWritable map_count = new IntWritable();

			public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
				StringTokenizer itr = new StringTokenizer(value.toString());
				while (itr.hasMoreTokens()) {
					word.set(itr.nextToken());
					context.write(word, one);
				}
			}
		}

	// reducer class
	public static class IntSumReducer
		extends Reducer<Text, IntWritable, Text, IntWritable> {
			private IntWritable result = new IntWritable();

			public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
				int sum = 0;
				for (IntWritable val : values) {
					sum += val.get();
				}
				result.set(sum);
				context.write(key, result);
			}
		}

	// main class
	public static void main(String[] args) throws Exception {
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		if (otherArgs.length != 2) {
			System.err.println("Usage: wordcount <in> <out>");
			System.exit(2);
		}

		Job job = new Job(conf, "word count");
		job.setNumReduceTasks(1);//设置Reducer个数
		job.setJarByClass(WordCount.class);
		job.setMapperClass(TokenizerMapper.class);
		job.setCombinerClass(IntSumReducer.class);
		job.setReducerClass(IntSumReducer.class);

		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));//设置输入数据的hdfs路径

		job.setOutputKeyClass(Text.class);//设置输出key类型
		job.setOutputValueClass(IntWritable.class);//设置输出value类型
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));//设置输出数据的hdfs路径

		System.exit(job.waitForCompletion(true) ? 0 : 1);

	}
}
