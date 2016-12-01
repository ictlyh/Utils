public class TestEscape {
  public static void main(String args[]) {
    String str1 = "row format delimited fields terminated by '\\001'";
    String str2 = "row format delimited fields terminated by '\134'";
    System.out.println(str1);
    System.out.println(str2);
  }
}
