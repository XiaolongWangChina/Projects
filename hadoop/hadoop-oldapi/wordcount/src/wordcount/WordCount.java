package wordcount;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapred.FileInputFormat;
import org.apache.hadoop.mapred.FileOutputFormat;
import org.apache.hadoop.mapred.JobClient;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.MapReduceBase;
import org.apache.hadoop.mapred.Mapper;
import org.apache.hadoop.mapred.OutputCollector;
import org.apache.hadoop.mapred.Reducer;
import org.apache.hadoop.mapred.Reporter;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class WordCount extends Configured implements Tool {

	public static class MapClass extends MapReduceBase
		implements Mapper<LongWritable, Text, Text, IntWritable> {
			private final static IntWritable one = new IntWritable(1);
			private Text word = new Text();
			public void map(LongWritable key, Text value,
					OutputCollector<Text, IntWritable> output,
					Reporter reporter) throws IOException {
				String line = value.toString();
				StringTokenizer itr = new StringTokenizer(line);
				while (itr.hasMoreTokens()) {
					word.set(itr.nextToken());
					output.collect(word, one);
				}
			}
		}

	public static class ReduceClass extends MapReduceBase
		implements Reducer<Text, IntWritable, Text, IntWritable> {
			public void reduce(Text key, Iterator<IntWritable> values,
					OutputCollector<Text, IntWritable> output,
					Reporter reporter) throws IOException {
				int sum = 0;
				while (values.hasNext()) {
					sum += values.next().get();
				}
				output.collect(key, new IntWritable(sum));
			}
		}

	public int run(String[] args) throws Exception {
		JobConf conf = new JobConf(getConf(), WordCount.class);
		conf.setJobName("WordCount");

		conf.setOutputKeyClass(Text.class);
		conf.setOutputValueClass(IntWritable.class);

		conf.setMapperClass(MapClass.class);
		conf.setCombinerClass(ReduceClass.class);
		conf.setReducerClass(ReduceClass.class);

		//conf.setNumMapTasks(2);
		//conf.setNumReduceTasks(1);
		FileInputFormat.setInputPaths(conf, args[0]);
		FileOutputFormat.setOutputPath(conf, new Path(args[1]));

		JobClient.runJob(conf);
		return 0;
	}

	public static void main(String[] args) throws Exception {
		int res = ToolRunner.run(new Configuration(), new WordCount(), args);
		System.exit(res);
	}
}
