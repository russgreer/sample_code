/*
   ABC Widgets application
   Programmer: Russ Greer
   Filename: ABCWidgets.java
   Purpose:  An application to manage ABC Computers widgets
*/

import javax.swing.*;
import java.awt.*;
import java.sql.*;
import javax.swing.table.*;
import java.awt.event.*;

public class ABCWidgets extends JPanel
{
    // Database variables
    String url = "jdbc:mysql://localhost:3306/";
    String dbName = "final";
    String driver = "com.mysql.jdbc.Driver";
    String userName = "root";
    String password = "password"; // local dev... change this .

    // JTable and JLabel variables
    DefaultTableModel defTableModel;
    JLabel economyLabel;
    JLabel standardLabel;
    JLabel advancedLabel;
    JLabel exceptionalLabel;

    // Counters to keep track of widget totals
    Integer economyTotal = 0;
    Integer standardTotal = 0;
    Integer advancedTotal = 0;
    Integer exceptionalTotal = 0;


    public ABCWidgets()
    {
		// create container panel to hold all other panels
        JPanel containerPanel = new JPanel(new GridLayout(3, 3));
        containerPanel.setPreferredSize(new Dimension(560, 250));


        // setup top panel to hold combo boxes, text field and "add" button
        JPanel topPanel = new JPanel(new GridLayout(1, 4));

        // setup style combobox
        final DefaultComboBoxModel style = new DefaultComboBoxModel();

        style.addElement("economy");
        style.addElement("standard");
        style.addElement("advanced");
        style.addElement("exceptional");

        final JComboBox styleCombo = new JComboBox(style);
        styleCombo.setSelectedIndex(0);
        topPanel.add(styleCombo);


        // setup color combobox
        final DefaultComboBoxModel color = new DefaultComboBoxModel();

        color.addElement("red");
        color.addElement("yellow");
        color.addElement("green");
        color.addElement("blue");

        final JComboBox colorCombo = new JComboBox(color);
        colorCombo.setSelectedIndex(0);
        topPanel.add(colorCombo);

        // Quantity
        final TextField quantityField = new TextField("1");
        topPanel.add(quantityField);

        // create and setup the add button
        JButton addBtn = new JButton("Add");
        addBtn.addActionListener(new ActionListener()
            {
	              public void actionPerformed(ActionEvent e)
	              {
					  // get combo box and textbox values
					  String styleStr = styleCombo.getSelectedItem().toString();
					  String colorStr = colorCombo.getSelectedItem().toString();
					  String quantity = quantityField.getText();

					  insertRowToDB(styleStr, colorStr, quantity );

                      // setup row to insert into JTable
                      Object[] row = new Object[3];
                      row[0] = styleStr;
                      row[1] = colorStr;
                      row[2] = quantity;
                      defTableModel.addRow(row);

                      // update labels of style totals
                      if(styleStr.equals("economy"))
                        {
                           economyTotal += Integer.parseInt(quantity);
                           economyLabel.setText("economy: " + economyTotal.toString());
					    }
                       else if(styleStr.equals("standard"))
                        {
                           standardTotal += Integer.parseInt(quantity);
                           standardLabel.setText("standard: " + standardTotal.toString());
					    }
                       else if(styleStr.equals("advanced"))
                        {
                           advancedTotal += Integer.parseInt(quantity);
                           advancedLabel.setText("advanced: " + advancedTotal.toString());
					    }
                       else if(styleStr.equals("exceptional"))
                        {
                           exceptionalTotal += Integer.parseInt(quantity);
                           exceptionalLabel.setText("exceptional: " + exceptionalTotal.toString());
		  			    }

	                }
              } );

        topPanel.add(addBtn);


        // set topPanel size
        topPanel.setMaximumSize(new Dimension(550,25));
        topPanel.setPreferredSize(new Dimension(550, 25));

        // add top panel to the container panel
        containerPanel.add(topPanel, BorderLayout.PAGE_START);


        // Add a spacer (JPanel) so the UI doesn't look cluttered
        JPanel spacer1 = new JPanel();
        spacer1.setMaximumSize(new Dimension(550,25));
        spacer1.setPreferredSize(new Dimension(550, 25));
        containerPanel.add(spacer1);


        // Create and setup the JTable
        JPanel middlePanel = new JPanel(new GridLayout(1, 0));

        String[] columnNames = { "style", "color", "quantity" };

        defTableModel = new DefaultTableModel(null, columnNames);
        JTable table = new JTable(defTableModel);

        table.setPreferredScrollableViewportSize(new Dimension(550, 170));

        // get rows from the database using custom get num rows method...
        Object[][] rows = getDBRows(getNumDBRows());

        // add DB rows to the JTable
        for(int i =0; i < rows.length; i++)
            defTableModel.addRow(rows[i]);

        // set the JTable colors
        table.setGridColor( Color.BLACK );
        table.setBackground( new Color(210, 244, 244) );

        // select first row in the JTable
        table.setRowSelectionInterval(0, 0);

        // set automatic sorting on the JTable
        table.setAutoCreateRowSorter(true);

        // Create the scroll pane and add the table to it.
        JScrollPane scrollPane = new JScrollPane(table);

        // Add the scroll pane to this panel.
        middlePanel.add(scrollPane);

        middlePanel.setPreferredSize(new Dimension(550, 170));

        // add the middel panel to the container panel
        containerPanel.add(middlePanel, BorderLayout.SOUTH);


        // add the labels to show totals for each style (regardless of color)
        JPanel spacer2 = new JPanel();
        spacer2.setMaximumSize(new Dimension(550,25));
        spacer2.setPreferredSize(new Dimension(550, 25));

        // setup the labels
        economyLabel = new JLabel("economy: " + (economyTotal = getTotalDBStyle("economy")));
        standardLabel = new JLabel("standard: " + (standardTotal = getTotalDBStyle("standard")));
        advancedLabel = new JLabel("advanced: " + (advancedTotal = getTotalDBStyle("advanced")));
        exceptionalLabel = new JLabel("exceptional: " + (exceptionalTotal = getTotalDBStyle("exceptional")));

        // set size of each label
        economyLabel.setPreferredSize(new Dimension(120, 25));
        standardLabel.setPreferredSize(new Dimension(120, 25));
        advancedLabel.setPreferredSize(new Dimension(120, 25));
        exceptionalLabel.setPreferredSize(new Dimension(120, 25));

        // add labels
        spacer2.add(economyLabel);
        spacer2.add(standardLabel);
        spacer2.add(advancedLabel);
        spacer2.add(exceptionalLabel);

        // add spacer panel to container panel
        containerPanel.add(spacer2);

        // set box layout
        containerPanel.setLayout(new BoxLayout(containerPanel, BoxLayout.Y_AXIS));

        // add container panel
        add(containerPanel);
    }




    // Get database rows to fill in the JTable
    public Object[][] getDBRows(int numRows)
    {
		Object[][] data = new Object[numRows][20]; // allocate 2 dimensional array

   		try
   		{
		  // connect to the database
		  Class.forName(driver).newInstance();
		  Connection conn = DriverManager.getConnection(url+dbName,userName,password);
		  Statement st = conn.createStatement();

		  // get all rows from the database
		  ResultSet res = st.executeQuery("SELECT * FROM widget ORDER BY widgetid");

          int i = 0;
		  while(res.next())
		  {
   		     Object[] row = new Object[3];  // allocate 3 array members in row[]

   		     row[1] = res.getObject(2);  // color
   		     row[0] = res.getObject(3);  // style
   		     row[2] = res.getObject(4);  // quantity

   		     data[i++] = row;
		  }

          st.close();
		  conn.close();
   		}
      catch (Exception e)
 		{
		  e.printStackTrace();
 	    }

      return data;
	}



    // Get the total number of rows from the database
    public int getNumDBRows()
    {
		int rowCount = 0;

   		try
   		{
		  // connect to the database
 		  Class.forName(driver).newInstance();
		  Connection conn = DriverManager.getConnection(url+dbName,userName,password);
		  Statement st = conn.createStatement();

		  // get row count
		  ResultSet res = st.executeQuery("SELECT COUNT(*) FROM widget");

		  if(res.next())
		     rowCount = res.getInt(1);  // get count of all rows

          st.close();
		  conn.close();
	    }
	  catch (Exception e)
 		{
		  e.printStackTrace();
 	    }

		return rowCount;
	}



    public void insertRowToDB(String style, String color, String quantity )
    {

		try
		{
		   // connect to the database
		   Class.forName(driver).newInstance();
		   Connection conn = DriverManager.getConnection(url+dbName,userName,password);
		   Statement st = conn.createStatement();

           // insert into the database
		   st.executeUpdate("INSERT into widget (color,style,quantity) VALUES ('"+color+"', '"+style+"', "+quantity+" )");

		   st.close();
		   conn.close();
		}
	  catch (Exception e)
		{
		   e.printStackTrace();
		}

	}



    // Gets the total quantity of the style type passed
    public int getTotalDBStyle(String style)
    {
		int count = 0;

		   		try
		   		{
			      // connect to the database
		 		  Class.forName(driver).newInstance();
				  Connection conn = DriverManager.getConnection(url+dbName,userName,password);
				  Statement st = conn.createStatement();

				  // get row count
				  ResultSet res = st.executeQuery("SELECT SUM(final.widget.quantity) FROM widget where style = '"+style+"' ");

				  if(res.next())
				     count = res.getInt(1); // get the total for this style widget

		          st.close();
				  conn.close();
			    }
			  catch (Exception e)
		 		{
				  e.printStackTrace();
		 	    }

		return count;
    }



    public static void main(String[] args)
    {
         // Create and set up the window
         JFrame frame = new JFrame("ABC Computers");
         frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

         // Create and set up the content pane.
         ABCWidgets newContentPane = new ABCWidgets();
         newContentPane.setOpaque(true);               // content panes must be opaque
         frame.setContentPane(newContentPane);

         // Display the window.
         frame.pack();
         frame.setVisible(true);
    }

}