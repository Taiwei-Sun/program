package ForUpload;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;

import javax.servlet.ServletException;
import javax.servlet.annotation.MultipartConfig;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.Part;



/**
 * Servlet implementation class UploadSevlet
 */
@MultipartConfig(location="D:/workspace")
@WebServlet("/UploadServlet")
public class UploadServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;	
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UploadServlet() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String str;
		String sav[]=new String[6];		
		int i=0;
		Connection conn=null;
		PreparedStatement stm=null;
		response.setContentType("text/html;charset=UTF-8");		
		
		try
		{
		Class.forName("com.mysql.jdbc.Driver");
		conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/mydb","root","1234");
		stm = conn.prepareStatement("insert into thetest1 values(?,?,?,?,?,?)");
		
		request.setCharacterEncoding("UTF-8");
		Part pt=request.getPart("file01");
		String filename=getFilename(pt);		
		pt.write(filename);
		String local=getLocal(pt);		
		BufferedReader reader=new BufferedReader(new FileReader(local));
		while((str=reader.readLine())!=null)
		{
			String cnt[]=str.split(",");
			
			for(String str1:cnt)
			{
				sav[i]=str1;
				if(i==5)i=0;
				else i++;
			}
			stm.setString(1,sav[0]);
			stm.setString(2,sav[1]);
			stm.setString(3,sav[2]);
			stm.setString(4,sav[3]);
			stm.setString(5,sav[4]);
			stm.setString(6,sav[5]);
			stm.executeUpdate();
		}
			reader.close();
			File theFile = new File(local);
			if (theFile.exists())
			{
			theFile.delete();	
			System.out.println(theFile+"刪除成功");
			}
			else System.out.println(theFile+"刪除失敗");
			}
		catch(ClassNotFoundException e)
		{
		System.out.println("找不到驅動程式");
		}
		catch (Exception e) {
		e.printStackTrace();
		}
		response.sendRedirect("UploadServlet2");
	}
	
	private String getFilename(Part pt)
	{
		String header=pt.getHeader("Content-Disposition");
		System.out.println("*header*="+header);	//
		String filename=header.substring(header.lastIndexOf("\\")+1,
				header.lastIndexOf("\""));
		System.out.println("*filename*="+filename);
		return filename;
	}
	
	private String getLocal(Part pt)
	{
		String header=pt.getHeader("Content-Disposition");
		String local=header.substring(header.lastIndexOf("=")+2,
				header.lastIndexOf("\""));
		System.out.println("*local*="+local);	//
		return local;
	}
	
	
}
