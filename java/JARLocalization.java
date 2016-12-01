// package com.ict.yhluo.utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * String replace in Java .class file.
 * Reference: Java Virtual Machine Specification CLASS file format
 * https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html
 *
 * @author luoyuanhao
 *
 */

public class JARLocalization {
  public static void localize(String path) {
    FileInputStream fis = null;
    FileOutputStream fos = null;
    long totalsize = 0;
    int aval_buf = 100;
    byte[] bs = new byte[aval_buf];
    try {
      // Output replaced content to file path.out
      fis = new FileInputStream(new File(path));
      fos = new FileOutputStream(new File(path + ".out"));
      System.out.println("Processing: " + path);

      // Skip magic, max and minus version, 8 bytes
      fis.read(bs, 0, 8);
      fos.write(bs, 0, 8);
      totalsize += 8;

      // Get number of constant pool entries, 2 bytes
      fis.read(bs, 0, 2);
      fos.write(bs, 0, 2);
      totalsize += 2;
      short cp_number = bytes2short(bs, 0, 2);
      System.out.println("Constant pool number: " + cp_number);

      // Handle each constant pool entry
      String str = null;
      for (short i = 1; i < cp_number; i++) {
        // Read flag, 1 byte
        fis.read(bs, 0, 1);
        fos.write(bs, 0, 1);
        totalsize += 1;
        // Unless tag value is 1(means utf-8_info where replacement
        // to be done), just skip specific bytes.
        short tag = bytes2short(bs, 0, 1);
        switch (tag) {
          case 7:
          case 8:
          case 16:
            fis.read(bs, 0, 2);
            fos.write(bs, 0, 2);
            totalsize += 2;
            break;
          case 15:
            fis.read(bs, 0, 3);
            fos.write(bs, 0, 3);
            totalsize += 3;
            break;
          case 3:
          case 4:
          case 9:
          case 10:
          case 11:
          case 12:
          case 18:
            fis.read(bs, 0, 4);
            fos.write(bs, 0, 4);
            totalsize += 4;
            break;
          case 5:
          case 6:
            fis.read(bs, 0, 8);
            fos.write(bs, 0, 8);
            //  Next cp index must be valid but is considered unusable
            i++;
            totalsize += 8;
            break;
          case 1: {
            fis.read(bs, 0, 2);
            totalsize += 2;
            short str_len = bytes2short(bs, 0, 2);
            while (str_len > aval_buf) {
              System.out.println("Constant pool number: " + i);
              System.out.println(
                  "Buffer overflow, double it from " + aval_buf + " to " + aval_buf * 2);
              aval_buf *= 2;
              bs = new byte[aval_buf];
            }
            fis.read(bs, 0, str_len);
            totalsize += str_len;
            // There may be '\0' in bytes array, but UTF-8 can't
            // handle it, so using 'ISO-8859-1' to encode string.
            str = new String(bs, 0, str_len, "ISO-8859-1");
            str = localizeInternal(str);
            str_len = (short) str.length();
            byte[] new_len = short2bytes(str_len);
            // Update string and length
            fos.write(new_len, 0, 2);
            fos.write(str.getBytes("ISO-8859-1"), 0, str_len);
            break;
          }
          default:
            System.out.println("File: " + path);
            System.out.println("Unrecognized tag: " + tag + ", cp num: " + i);
            System.out.println("After: " + str + ". Byte offset:" + totalsize);
            System.exit(1);
        } // end switch
      } // end for
      // Read rests
      byte[] bsrest = new byte[fis.available()];
      fis.read(bsrest);
      fos.write(bsrest);
    } catch (Exception e) {
      e.printStackTrace();
    } finally {
      if (fis != null) {
        try {
          fis.close();
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
      if (fos != null) {
        try {
          fos.close();
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
    }
  }

  private static short bytes2short(byte[] bs, int offset, int length) {
    if (length == 1) return (short) (bs[0] & 0xFF);
    ByteBuffer buf = ByteBuffer.wrap(bs, offset, length);
    buf.order(ByteOrder.BIG_ENDIAN);
    return buf.getShort();
  }

  private static byte[] short2bytes(short val) {
    ByteBuffer buf = ByteBuffer.allocate(2);
    buf.putShort(val);
    return buf.array();
  }
  private static String localizeInternal(String str) {
    // Replace "hello" whih "world"
    String new_str = str.replaceFirst("hello", "world");
    while (!new_str.equals(str)) {
      str = new_str;
      new_str = str.replaceFirst("hello", "world");
    }
    return str;
  }

  public static void main(String args[]) {
    if (args.length != 1) {
      System.out.println("You must specify the prefix");
      System.exit(1);
    }
    String line;
    String prefix = args[0];
    BufferedReader br = null;
    try {
      // class files are listed in prefix/files.list
      br = new BufferedReader(
          new InputStreamReader(new FileInputStream(prefix + "files.list")));
    } catch (Exception e) {
      e.printStackTrace();
      System.exit(1);
    }
    try {
      int i = 0;
      while ((line = br.readLine()) != null) {
        // Perform string replacement for each class file
        localize(prefix + line);
        i++;
      }
      System.out.println("Total file number: " + i);
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
