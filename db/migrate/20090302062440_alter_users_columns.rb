class AlterUsersColumns < ActiveRecord::Migration
  def self.up
    change_column :users, :name, :string, :null => false
    rename_column :users, :name, :first_name
    rename_column :users, :login, :username
    add_column :users, :last_name, :string, :limit => 50
    add_column :users, :profile_id, :integer, :options => "auto_increment = 1"
    #change_column :users, :profile_id, :integer, :null => false
    
  end

  def self.down
    remove_column :users, :profile_id
    remove_column :users, :last_name
    rename_column :users, :username, :login
    rename_column :users, :first_name, :name
    change_column :users, :name, :null=>true
  end
end
