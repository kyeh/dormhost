class AlterUserEditDetails < ActiveRecord::Migration
  def self.up
    add_column :users, :password_reset_code, :string, :limit => 40
    add_column :users, :user_type, :string, :limit => 40
  end

  def self.down
    remove_column :users, :password_reset_code
    remove_column :users, :user_type
  end
end
