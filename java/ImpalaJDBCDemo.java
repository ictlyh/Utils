import java.sql.*;
import java.util.Properties;

/**
 * This is a simple demo of Impala JDBC.
 * It shows how to:
 * 1. get system info of database
 * 2. get catalog, schema info
 * 3. get table, column, function, index info. etc.
 * 4. DML/DDL statement
 *
 *
 * Major classes and interfaces in package java.sql:
 * Connection, DatabaseMetaData, Driver, ResultSet
 * Statement, CallableStatement and PreparedStatement.
 * To learn more about these, refer to:
 * http://docs.oracle.com/javase/7/docs/api/
 *
 * To learn more about Impala JDBC, refer to:
 * https://www.cloudera.com/content/www/en-us/documentation/other/connectors/
 * impala-jdbc/latest/Cloudera-JDBC-Driver-for-Impala-Install-Guide.pdf
 */
public class ImpalaJDBCDemo {
  // Define a string as the fully qualified class name
  //(FQCN) of the desired JDBC driver
  static String JDBCDriver = "com.cloudera.impala.jdbc41.Driver";

  // Define a string as the connection URL
  private static final String CONNECTION_URL = "jdbc:impala://nobida147:21050/test";

  public static void main(String[] args) {
    Connection con = null;
    Statement stmt = null;
    PreparedStatement prep = null;
    ResultSet rs = null;

    // Make connection to database
    try {
      // Register the driver using the class name
      Class.forName(JDBCDriver);

      // Establish a connection using the connection URL
      con = DriverManager.getConnection(CONNECTION_URL);

      System.out.println("Catalog of connection: " + con.getCatalog());
      System.out.println("Schema of connection:" + con.getSchema());
      con.setClientInfo("APPLICATIONNAME", "Impala JDBC demo");
      con.setClientInfo("CLIENTUSER", "luoyuanhao");
      //      con.setClientInfo("CLIENTHOSTNAME ", "Nobida147"); // fail
      Properties clientInfo = con.getClientInfo();
      clientInfo.list(System.out);

      DatabaseMetaData dbMeta = con.getMetaData();

      // Client info
      rs = dbMeta.getClientInfoProperties();
      while (rs.next()) {
        String key = rs.getString(1);
        String value = rs.getString(2);
        System.out.println(key + " : " + value);
      }
      if (rs != null) rs.close();

      // Catalogs available in this database. Only "Impala"
      rs = dbMeta.getCatalogs();
      while (rs.next()) {
        System.out.println(rs.getString(1));
      }
      if (rs != null) rs.close();

      // Catalog seperator(catalog.schema.table) and term
      System.out.println("Catalog seperator: " + dbMeta.getCatalogSeparator()); // "."
      System.out.println("Catalog term: " + dbMeta.getCatalogTerm()); // "catalog"

      // Column attributes of table 't1' in database 'test'
      rs = dbMeta.getColumns(null, "test", "t1", null);
      System.out.printf(
          "%10s%10s%10s%10s%10s\n", "Catalog", "Schema", "Table", "Column", "Type name");
      while (rs.next()) {
        String cat = rs.getString(1);
        String sche = rs.getString(2);
        String tb = rs.getString(3);
        String col = rs.getString(4);
        String type = rs.getString(6);
        System.out.printf("%10s%10s%10s%10s%10s\n", cat, sche, tb, col, type);
      }
      if (rs != null) rs.close();

      // Column privilege
      rs = dbMeta.getColumnPrivileges(null, "test", "t1", null);
      System.out.printf("%10s%10s%10s%10s%10s%10s%10s\n", "Catalog", "Schema", "Table",
          "Column", "Grantor", "Grantee", "Privilege");
      while (rs.next()) {
        String cat = rs.getString(1);
        String sche = rs.getString(2);
        String tb = rs.getString(3);
        String col = rs.getString(4);
        String grantor = rs.getString(5);
        String grantee = rs.getString(6);
        String privilege = rs.getString(7);
        System.out.printf("%10s%10s%10s%10s%10s%10s%10s\n", cat, sche, tb, col, grantor,
            grantee, privilege);
      }
      if (rs != null) rs.close();

      System.out.println("Database major version: " + dbMeta.getDatabaseMajorVersion());
      System.out.println("Database minor version:" + dbMeta.getDatabaseMinorVersion());
      System.out.println("Database product name:" + dbMeta.getDatabaseProductName());
      System.out.println(
          "Database product version: " + dbMeta.getDatabaseProductVersion());
      System.out.println("Driver name: " + dbMeta.getDriverName());
      System.out.println("Driver version: " + dbMeta.getDriverVersion());
      System.out.println("Driver major version:" + dbMeta.getDriverMajorVersion());
      System.out.println("Driver minor version:" + dbMeta.getDriverMinorVersion());

      // Other useful methods:
      // DatabaseMetaData.getTables()
      // DatabaseMetaData.getFunctions();
      // DatabaseMetaData.getIndexInfo();
    } catch (Exception e) {
      e.printStackTrace();
    }

    // Plain statement
    try {
      stmt = con.createStatement();

      // All 0, why? Set them first?
      System.out.println("Statement fetch size: " + stmt.getFetchSize());
      System.out.println("Max result set number: " + stmt.getLargeMaxRows());
      System.out.println("Max field size:" + stmt.getMaxFieldSize());
      System.out.println("Max rows:" + stmt.getMaxRows());
      System.out.println("Query time out:" + stmt.getQueryTimeout());

      // Plain query
      rs = stmt.executeQuery("SELECT * FROM t1 WHERE id = 1");
      while (rs.next()) {
        Long id = rs.getLong("id");
        String name = rs.getString("name");
        System.out.printf("%10s%10s\r\n", id.toString(), name);
      }
      if (rs != null) rs.close();

      // Plain insert
      stmt.executeUpdate("INSERT INTO t1 VALUES(4,'plain insert')");

      // Create function
      stmt.executeUpdate("CREATE FUNCTION my_add_new(int, int) returns int" +
      " location 'file:///home/lyh/workspace/utils/impala-udf-samples-master/build/libudfsample.so'"
          + " SYMBOL='AddUdf'");

      // Drop function
      stmt.executeUpdate("DROP FUNCTION my_add_new(int,int)");

      // Call function
      rs = stmt.executeQuery("SELECT my_add(id, 0) FROM t1");
      while (rs.next()) {
        System.out.printf("%10d\n", rs.getLong(1));
      }
      if (rs != null) rs.close();

      // Create database
      stmt.executeUpdate("CREATE DATABASE db1 pglocation('10.60.1.147:5432')");

      // Drop database
      stmt.executeUpdate("DROP DATABASE db1");

      // Create index
      stmt.executeUpdate("CREATE INDEX idx ON t1(id)");

      // Drop index
      stmt.executeUpdate("DROP INDEX idx");

      // Create table
      stmt.executeUpdate("CREATE TABLE t2(name string, age int)");

      // Drop table
      stmt.executeUpdate("DROP TABLE t2");

      if (stmt != null) stmt.close();
    } catch (Exception e) {
      e.printStackTrace();
    }

    // Prepared statement
    try {
      // Prepared query
      String preStmt = "SELECT * FROM t1 WHERE id = ?";
      prep = con.prepareStatement(preStmt);
      prep.setInt(1, 4);
      rs = prep.executeQuery();
      while (rs.next()) {
        Long id = rs.getLong("id");
        String name = rs.getString("name");
        System.out.printf("%10s%10s\r\n", id.toString(), name);
      }
      if (rs != null) rs.close();

      // Prepared insert
      preStmt = "INSERT INTO t1 VALUES(?,?)";
      prep = con.prepareStatement(preStmt);
      prep.setInt(1, 5);
      prep.setString(2, "prepared insert");
      prep.execute();
    } catch (Exception e) {
      e.printStackTrace();
    }
    try {
      if (con != null) con.close();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
