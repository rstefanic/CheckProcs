# CheckProcs
A tool to analyze code and see what procedures are not being used.

Uses CLR objects to connect to SQL server.

# Usage
Can be used by adding a file called "connectionString" where the project sits which just includes the connection string. 

Then you can analyze a project by passing the name of the database that you want to check your project against, followed by the path to your project with the appropriate flags.

```
'./Check Procs' -n database -s /home/robert/project/
```
