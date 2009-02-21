class CreateUsers < ActiveRecord::Migration
  def self.up
    create_table :users do |t|
      t.column  :profile_id,  :integer, :null => false
      t.column  :first_name,  :string,  :limit => 50
      t.column  :last_name,   :string,  :limit => 50
      t.column  :username,    :string,  :limit => 25
      t.column  :password,    :string,  :limit => 25
      t.column  :email,       :string,  :limit => 50
      t.column  :created_at,  :timestamp

      t.timestamps
    end
  end

  def self.down
    drop_table :users
  end
end
